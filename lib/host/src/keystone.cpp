//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/mman.h>
#include <keystone_user.h>
#include "keystone.h"
#include "elffile.h"
#include "keystone_user.h"
#include "page.h"
#include "hash_util.h"
#include <math.h>

Keystone::Keystone() {
  runtimeFile = NULL;
  enclaveFile = NULL;
  untrusted_size = 0;
  untrusted_start = 0;
  epm_free_list = 0;
  root_page_table = 0;
  start_addr = 0;
  eid = -1;

}

Keystone::~Keystone() {
  if(runtimeFile)
    delete runtimeFile;
  if(enclaveFile)
    delete enclaveFile;
  destroy();
}

unsigned long calculate_required_pages(
        unsigned long eapp_sz,
        unsigned long rt_sz) {
    unsigned long req_pages = 0;

    req_pages += ceil(eapp_sz / PAGE_SIZE);
    req_pages += ceil(rt_sz / PAGE_SIZE);

    /* FIXME: calculate the required number of pages for the page table.
     * We actually don't know how many page tables the enclave might need,
     * because the SDK never knows how its memory will be aligned.
     * Ideally, this should be managed by the driver.
     * For now, we naively allocate enough pages so that we can temporarily get away from this problem.
     * 15 pages will be more than sufficient to cover several hundreds of megabytes of enclave/runtime. */
    req_pages += 15;
    return req_pages;
}


keystone_status_t Keystone::loadUntrusted() {
    vaddr_t va_start = ROUND_DOWN(untrusted_start, PAGE_BITS);
    vaddr_t va_end = ROUND_UP(untrusted_start + untrusted_size, PAGE_BITS);
    static char nullpage[PAGE_SIZE] = {0,};

    while (va_start < va_end) {
        if (allocPage(va_start, &utm_free_list, (vaddr_t) nullpage, UTM_FULL) == KEYSTONE_ERROR){
          PERROR("failed to add page - allocPage() failed");
        }
        va_start += PAGE_SIZE;
    }
    return KEYSTONE_SUCCESS;
}

/* This function will be deprecated when we implement freemem */
keystone_status_t Keystone::initStack(vaddr_t start, size_t size, bool is_rt)
{
  static char nullpage[PAGE_SIZE] = {0,};
  vaddr_t high_addr = ROUND_UP(start, PAGE_BITS);
  vaddr_t va_start_stk = ROUND_DOWN((high_addr - size), PAGE_BITS);
  int stk_pages = (high_addr - va_start_stk) / PAGE_SIZE;

  for (int i = 0; i < stk_pages; i++) {
    if (allocPage(va_start_stk,  &epm_free_list, (vaddr_t) nullpage, (is_rt ? RT_NOEXEC : USER_NOEXEC)) == KEYSTONE_ERROR)
      return KEYSTONE_ERROR;

    va_start_stk += PAGE_SIZE;
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::allocPage(vaddr_t va, vaddr_t *free_list, vaddr_t src, unsigned int mode)
{

  vaddr_t page_addr;

  pte_t* pte = __ept_walk_create(pMemory, &epm_free_list, (pte_t *) root_page_table, va);

  /* if the page has been already allocated, return the page */
  if(pte_val(*pte) & PTE_V) {
      return KEYSTONE_SUCCESS;
  }

  /* otherwise, allocate one from EPM freelist */
  page_addr = *free_list >> PAGE_BITS;
  *free_list += PAGE_SIZE;

  switch (mode) {
    case USER_NOEXEC: {
      *pte = pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_U | PTE_V);
      break;
    }
    case RT_NOEXEC: {
      *pte = pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_V);
      break;
    }
    case RT_FULL: {
      *pte = pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_V);
      pMemory->WriteMem(src, (vaddr_t) page_addr << PAGE_BITS, PAGE_SIZE);
      break;
  }
    case USER_FULL: {
      *pte = pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_U | PTE_V);
      pMemory->WriteMem(src, (vaddr_t) page_addr << PAGE_BITS, PAGE_SIZE);
      break;
    }
    case UTM_FULL: {
      *pte = pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W |PTE_V);
      pMemory->WriteMem(src, (vaddr_t) page_addr << PAGE_BITS, PAGE_SIZE);
      break;
    }
    default: {
      PERROR("failed to add page - mode is invalid");
      return KEYSTONE_ERROR;
    }
  }

  return KEYSTONE_SUCCESS;

}

keystone_status_t Keystone::loadELF(ELFFile* elf, uintptr_t* data_start)
{
  static char nullpage[PAGE_SIZE] = {0,};
  unsigned int mode = elf->getPageMode();
  vaddr_t va;

  size_t num_pages = ROUND_DOWN(elf->getTotalMemorySize(), PAGE_BITS) / PAGE_SIZE;
  va = elf->getMinVaddr();

  if (epm_alloc_vspace(pMemory, &epm_free_list, (pte_t *) root_page_table, va, num_pages) != num_pages)
  {
    ERROR("failed to allocate vspace\n");
    return KEYSTONE_ERROR;
  }
  *data_start = epm_free_list;
  for (unsigned int i = 0; i < elf->getNumProgramHeaders(); i++) {

    if (elf->getProgramHeaderType(i) != PT_LOAD) {
      continue;
    }

    vaddr_t start = elf->getProgramHeaderVaddr(i);
    vaddr_t file_end = start + elf->getProgramHeaderFileSize(i);
    vaddr_t memory_end = start + elf->getProgramHeaderMemorySize(i);
    char* src = (char*) elf->getProgramSegment(i);
    va = start;

    /* FIXME: This is a temporary fix for loading iozone binary
     * which has a page-misaligned program header. */
    if(!IS_ALIGNED(va, PAGE_SIZE)) {
      size_t offset = va - PAGE_DOWN(va);
      size_t length = PAGE_UP(va) - va;
      char page[PAGE_SIZE];
      memset(page, 0, PAGE_SIZE);
      memcpy(page + offset, (const void*) src, length);
      if (allocPage(PAGE_DOWN(va), &epm_free_list, (vaddr_t) page, mode) != KEYSTONE_SUCCESS)
        return KEYSTONE_ERROR;
      va += length;
      src += length;
    }

    /* first load all pages that do not include .bss segment */
    while (va + PAGE_SIZE <= file_end) {
      if (allocPage(va, &epm_free_list, (vaddr_t) src, mode) != KEYSTONE_SUCCESS)
        return KEYSTONE_ERROR;

      src += PAGE_SIZE;
      va += PAGE_SIZE;
    }

    /* next, load the page that has both initialized and uninitialized segments */
    if (va < file_end) {
      char page[PAGE_SIZE];
      memset(page, 0, PAGE_SIZE);
      memcpy(page, (const void*) src, (size_t) (file_end - va));
      if (allocPage(va,  &epm_free_list, (vaddr_t) page, mode) != KEYSTONE_SUCCESS)
        return KEYSTONE_ERROR;
      va += PAGE_SIZE;
    }

    /* finally, load the remaining .bss segments */
    while (va < memory_end)
    {
      if (allocPage(va,  &epm_free_list, (vaddr_t) nullpage, mode) != KEYSTONE_SUCCESS)
        return KEYSTONE_ERROR;
      va += PAGE_SIZE;
    }
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::validate_and_hash_enclave(struct runtime_params_t args,
                                           struct keystone_hash_enclave* cargs){

  hash_ctx_t hash_ctx;
  int ptlevel = RISCV_PGLEVEL_TOP;

  hash_init(&hash_ctx);

  // hash the runtime parameters
  hash_extend(&hash_ctx, &args, sizeof(struct runtime_params_t));


  uintptr_t runtime_max_seen=0;
  uintptr_t user_max_seen=0;

  // hash the epm contents including the virtual addresses
  int valid = validate_and_hash_epm(&hash_ctx,
                                    ptlevel,
                                    (pte_t*) root_page_table,
                                    0, 0, cargs, &runtime_max_seen, &user_max_seen, fd);

  if(valid == -1){
    return KEYSTONE_ERROR;
  }

  hash_finalize(hash, &hash_ctx);

  return KEYSTONE_SUCCESS;
}

bool Keystone::initFiles(const char* eapppath, const char* runtimepath)
{
  if (runtimeFile || enclaveFile) {
    ERROR("ELF files already initialized");
    return false;
  }

  runtimeFile = new ELFFile(runtimepath);
  enclaveFile = new ELFFile(eapppath);

  if(!runtimeFile->initialize(true)) {
    ERROR("Invalid runtime ELF\n");
    destroy();
    return false;
  }

  if(!enclaveFile->initialize(false)) {
    ERROR("Invalid enclave ELF\n");
    destroy();
    return false;
  }

  if (!runtimeFile->isValid()) {
    ERROR("runtime file is not valid");
    destroy();
    return false;
  }
  if (!enclaveFile->isValid()) {
    ERROR("enclave file is not valid");
    destroy();
    return false;
  }

  return true;
}

bool Keystone::prepareEnclave(struct keystone_ioctl_create_enclave* enclp,
                              uintptr_t alternate_phys_addr)
{
  enclp->params.runtime_entry = (unsigned long) runtimeFile->getEntryPoint();
  enclp->params.user_entry = (unsigned long) enclaveFile->getEntryPoint();
  enclp->params.untrusted_ptr = (unsigned long) params.getUntrustedMem();
  enclp->params.untrusted_size = (unsigned long) params.getUntrustedSize();

  // FIXME: this will be deprecated with complete freemem support.
  // We just add freemem size for now.
  enclp->min_pages = ROUND_UP(params.getFreeMemSize(), PAGE_BITS)/PAGE_SIZE;
  enclp->min_pages += calculate_required_pages(enclaveFile->getTotalMemorySize(),
      runtimeFile->getTotalMemorySize());
  enclp->runtime_vaddr = (unsigned long) runtimeFile->getMinVaddr();
  enclp->user_vaddr = (unsigned long) enclaveFile->getMinVaddr();

  untrusted_size = params.getUntrustedSize();
  untrusted_start = params.getUntrustedMem();

  if (params.isSimulated()) {
    eid = -1; // simulated
    pMemory->init(0, 0);
    root_page_table = pMemory->AllocMem(PAGE_SIZE * enclp->min_pages);
    start_addr = root_page_table;
    epm_free_list = start_addr + PAGE_SIZE;
    return true;
  }

  /* Pass in pages to map to enclave here. */

  /* Call Keystone Driver */
  int ret = kDevice->ioctl_ioc_create_enclave(enclp);

  if (ret) {
    ERROR("failed to create enclave - ioctl() failed: %d", ret);
    return false;
  }

  /* We switch out the phys addr as needed */

  uintptr_t starting_phys_range;
  if(alternate_phys_addr){
    //printf("Switching prev. %08llx for %08lx\n",enclp->pt_ptr, alternate_phys_addr);
    starting_phys_range = alternate_phys_addr;
  }
  else{
    starting_phys_range = enclp->pt_ptr;
  }

  pMemory->init(fd, starting_phys_range);
  eid = enclp->eid;
  start_addr = starting_phys_range;
  root_page_table = pMemory->AllocMem(PAGE_SIZE);
  epm_free_list = starting_phys_range + PAGE_SIZE;
  return true;
}

keystone_status_t Keystone::init(const char *eapppath, const char *runtimepath, Params _params){
  return this->init(eapppath, runtimepath, _params, (uintptr_t)0);
}

const char* Keystone::getHash(){
  return this->hash;
}

keystone_status_t Keystone::init(const char *eapppath, const char *runtimepath, Params _params, uintptr_t alternate_phys_addr)
{
  params = _params;

  if (params.isSimulated()) {
    pMemory = new SimulatedEnclaveMemory();
    kDevice = new MockKeystoneDevice();
  } else {
    pMemory = new PhysicalEnclaveMemory();
    kDevice = new KeystoneDevice();
  }

  if(!initFiles(eapppath, runtimepath)) {
    return KEYSTONE_ERROR;
  }

  if(!kDevice->initDevice(params)) {
    destroy();
    return KEYSTONE_ERROR;
  }

  fd = kDevice->getFD();

  struct keystone_ioctl_create_enclave enclp;
  if(!prepareEnclave(&enclp, alternate_phys_addr)) {
    destroy();
    return KEYSTONE_ERROR;
  }

  //Map root page table to user space
  struct keystone_hash_enclave hash_enclave;

  uintptr_t data_start;

  hash_enclave.runtime_paddr = epm_free_list;
  if(loadELF(runtimeFile, &data_start) != KEYSTONE_SUCCESS) {
    ERROR("failed to load runtime ELF");
    destroy();
    return KEYSTONE_ERROR;
  }
  enclp.runtime_paddr = (data_start - start_addr) + enclp.pt_ptr;

  hash_enclave.user_paddr = epm_free_list;
  if(loadELF(enclaveFile, &data_start) != KEYSTONE_SUCCESS) {
    ERROR("failed to load enclave ELF");
    destroy();
    return KEYSTONE_ERROR;
  }
  enclp.user_paddr = (data_start - start_addr) + enclp.pt_ptr;

  /* initialize stack. If not using freemem */
#ifndef USE_FREEMEM
  if( initStack(DEFAULT_STACK_START, DEFAULT_STACK_SIZE, 0) != KEYSTONE_SUCCESS){
    ERROR("failed to init static stack");
    destroy();
    return KEYSTONE_ERROR;
  }
#endif /* USE_FREEMEM */
  if(params.isSimulated()) {
    utm_free_list = pMemory->AllocMem(enclp.params.untrusted_size);
    hash_enclave.free_paddr = epm_free_list;
    hash_enclave.utm_paddr = utm_free_list;
  } else {
    int ret;
    ret = kDevice->ioctl_ioc_utm_init(&enclp);
    if (ret) {
      ERROR("failed to init untrusted memory - ioctl() failed: %d", ret);
      destroy();
      return KEYSTONE_ERROR;
    }
    utm_free_list = enclp.utm_free_ptr;
  }

  loadUntrusted();
  enclp.free_paddr = (epm_free_list - start_addr) + enclp.pt_ptr;
  if(params.isSimulated()) {
    hash_enclave.utm_size = params.getUntrustedSize();
    hash_enclave.epm_size = PAGE_SIZE * enclp.min_pages;
    hash_enclave.epm_paddr = root_page_table;
    hash_enclave.untrusted_ptr = enclp.params.untrusted_ptr;
    hash_enclave.untrusted_size = enclp.params.untrusted_size;

    validate_and_hash_enclave(enclp.params, &hash_enclave);
  } else {
    int ret;
    ret = kDevice->ioctl_ioc_finalize_enclave(&enclp);

    if (ret) {
      ERROR("failed to finalize enclave - ioctl() failed: %d", ret);
      destroy();
      return KEYSTONE_ERROR;
    }
    if (mapUntrusted(params.getUntrustedSize()))
    {
      ERROR("failed to finalize enclave - cannot obtain the untrusted buffer pointer \n");
      destroy();
      return KEYSTONE_ERROR;
    }
  }

  /* ELF files are no longer needed */
  delete enclaveFile;
  delete runtimeFile;
  enclaveFile = NULL;
  runtimeFile = NULL;
  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::mapUntrusted(size_t size)
{
  if (size == 0) {
    return KEYSTONE_SUCCESS;
  }

  shared_buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shared_buffer == NULL) {
    return KEYSTONE_ERROR;
  }

  shared_buffer_size = size;

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::destroy()
{
  /* if the enclave has ever created, we destroy it. */
  if(eid >= 0)
  {
    struct keystone_ioctl_create_enclave enclp;
    enclp.eid = eid;
    int ret = kDevice->ioctl_destroy_enclave(&enclp);

    if (ret) {
      ERROR("failed to destroy enclave - ioctl() failed: %d", ret);
      return KEYSTONE_ERROR;
    }
  }

  if(enclaveFile) {
    delete enclaveFile;
    enclaveFile = NULL;
  }

  if(runtimeFile) {
    delete runtimeFile;
    runtimeFile = NULL;
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::run()
{
  int ret;
  if (params.isSimulated()) {
    return KEYSTONE_SUCCESS;
  }

  struct keystone_ioctl_run_enclave run;
  run.eid = eid;

  ret = kDevice->ioctl_run_enclave(&run);
  while (ret == KEYSTONE_ENCLAVE_EDGE_CALL_HOST || ret == KEYSTONE_ENCLAVE_INTERRUPTED) {
    /* enclave is stopped in the middle. */
    if(ret == KEYSTONE_ENCLAVE_EDGE_CALL_HOST && oFuncDispatch != NULL) {
      oFuncDispatch(getSharedBuffer());
    }
    ret = kDevice->ioctl_resume_enclave(&run);
  }

  if (ret) {
    ERROR("failed to run enclave - ioctl() failed: %d", ret);
    destroy();
    return KEYSTONE_ERROR;
  }

  return KEYSTONE_SUCCESS;
}

void *Keystone::getSharedBuffer() {
  return shared_buffer;
}

size_t Keystone::getSharedBufferSize() {
  return shared_buffer_size;
}

keystone_status_t Keystone::registerOcallDispatch(OcallFunc func) {
  oFuncDispatch = func;
  return KEYSTONE_SUCCESS;
}

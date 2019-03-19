//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/mman.h>
//#include <linux/elf.h>
#include <keystone_user.h>
#include "keystone.h"
#include "elffile.h"
#include "keystone_user.h"
#include <math.h>

Keystone::Keystone() {
    runtimeFile = NULL;
    enclaveFile = NULL;
    enclave_stk_sz = 0;
    enclave_stk_start = 0;
    runtime_stk_sz = 0;
    untrusted_size = 0;
    untrusted_start = 0;
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
        unsigned long eapp_stack_sz,
        unsigned long rt_sz,
        unsigned long rt_stack_sz) {
    unsigned long req_pages = 0;

    req_pages += ceil(eapp_sz / PAGE_SIZE);
    req_pages += ceil(eapp_stack_sz / PAGE_SIZE);
    req_pages += ceil(rt_sz / PAGE_SIZE);
    req_pages += ceil(rt_stack_sz / PAGE_SIZE);

    /* FIXME: calculate the required number of pages for the page table.
     * We actually don't know how many page tables the enclave might need,
     * because the SDK never knows how its memory will be aligned.
     * Ideally, this should be managed by the driver.
     * For now, we naively allocate enough pages so that we can temporarily get away from this problem.
     * 15 pages will be more than sufficient to cover several hundreds of megabytes of enclave/runtime. */
    req_pages += 15;
    return req_pages;
}


keystone_status_t Keystone::loadUntrusted(void) {
    vaddr_t va_start = ROUND_DOWN(untrusted_start, PAGE_BITS);
    vaddr_t va_end = ROUND_UP(untrusted_start + untrusted_size, PAGE_BITS);

    while (va_start < va_end) {
        struct addr_packed encl_page;
        encl_page.va = va_start;
        encl_page.eid = eid;
        encl_page.mode = UTM_FULL;

        if (ioctl(fd, KEYSTONE_IOC_UTM_ALLOC, &encl_page)) {
            PERROR("failed to add page - ioctl() failed");
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
    if (allocPage(va_start_stk, nullpage, (is_rt ? RT_NOEXEC : USER_NOEXEC)))
      return KEYSTONE_ERROR;

    va_start_stk += PAGE_SIZE;
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::allocPage(vaddr_t va, void* src, unsigned int mode)
{
  struct addr_packed encl_page;
  encl_page.copied = (vaddr_t) src; // need to change to void*
  encl_page.va = va;
  encl_page.eid = eid;
  encl_page.mode = mode;

  if (ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &encl_page)) {
    PERROR("failed to add page - ioctl() failed");
    destroy();
    return KEYSTONE_ERROR;
  }
  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::loadELF(ELFFile* elf)
{
  static char nullpage[PAGE_SIZE] = {0,};
  unsigned int mode = elf->getPageMode();

  /* reserve virtual address space for linear mapping */
  struct keystone_ioctl_alloc_vspace vspace;
  vspace.eid = eid;
  vspace.vaddr = elf->getMinVaddr();
  vspace.size = elf->getTotalMemorySize();
  if(ioctl(fd, KEYSTONE_IOC_ALLOC_VSPACE, &vspace)) {
    PERROR("failed to reserve vspace - ioctl() failed");
    destroy();
    return KEYSTONE_ERROR;
  }

  for (unsigned int i = 0; i < elf->getNumProgramHeaders(); i++) {

    if (elf->getProgramHeaderType(i) != PT_LOAD) {
      continue;
    }

    vaddr_t start = elf->getProgramHeaderVaddr(i);
    vaddr_t file_end = start + elf->getProgramHeaderFileSize(i);
    vaddr_t memory_end = start + elf->getProgramHeaderMemorySize(i);
    char* src = (char*) elf->getProgramSegment(i);
    vaddr_t va = start;

    /* first load all pages that do not include .bss segment */
    while (va + PAGE_SIZE <= file_end) {
      if (allocPage(va, src, mode) != KEYSTONE_SUCCESS)
        return KEYSTONE_ERROR;

      src += PAGE_SIZE;
      va += PAGE_SIZE;
    }

    /* next, load the page that has both initialized and uninitialized segments */
    if (va < file_end) {
      char page[PAGE_SIZE];
      memset(page, 0, PAGE_SIZE);
      memcpy(page, (const void*) src, (size_t) (file_end - va));
      if (allocPage(va, page, mode) != KEYSTONE_SUCCESS)
        return KEYSTONE_ERROR;
      va += PAGE_SIZE;
    }

    /* finally, load the remaining .bss segments */
    while (va < memory_end)
    {
      if (allocPage(va, nullpage, mode) != KEYSTONE_SUCCESS)
        return KEYSTONE_ERROR;
      va += PAGE_SIZE;
    }
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::init(const char *eapppath, const char *runtimepath, Params params)
{
  if (runtimeFile || enclaveFile) {
    ERROR("ELF files already initialized");
    return KEYSTONE_ERROR;
  }

  runtimeFile = new ELFFile(runtimepath);
  enclaveFile = new ELFFile(eapppath);

  if(!runtimeFile->initialize(true)) {
    ERROR("Invalid runtime ELF\n");
    destroy();
    return KEYSTONE_ERROR;
  }

  if(!enclaveFile->initialize(false)) {
    ERROR("Invalid enclave ELF\n");
    destroy();
    return KEYSTONE_ERROR;
  }

  /* Enclave stack starts from runtime minVaddr.
   * This will be deprecated with freemem support */
  enclave_stk_start = runtimeFile->getMinVaddr();

  /* open device driver */
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if (fd < 0) {
    PERROR("cannot open device file");
    destroy();
    return KEYSTONE_ERROR;
  }

  if (!runtimeFile->isValid()) {
    ERROR("runtime file is not valid");
    destroy();
    return KEYSTONE_ERROR;
  }
  if (!enclaveFile->isValid()) {
    ERROR("enclave file is not valid");
    destroy();
    return KEYSTONE_ERROR;
  }

  /* Call Keystone Driver */
  struct keystone_ioctl_create_enclave enclp;

  enclp.params.runtime_entry = (unsigned long) runtimeFile->getEntryPoint();
  enclp.params.user_entry = (unsigned long) enclaveFile->getEntryPoint();
  enclp.params.untrusted_ptr = (unsigned long) params.getUntrustedMem();
  enclp.params.untrusted_size = (unsigned long) params.getUntrustedSize();

  // FIXME: this will be deprecated with complete freemem support.
  // We just add freemem size for now.
  enclp.min_pages = ROUND_UP(params.getFreeMemSize(), PAGE_BITS)/PAGE_SIZE;
  enclp.min_pages += calculate_required_pages(enclaveFile->getTotalMemorySize(), params.getEnclaveStack(),
      runtimeFile->getTotalMemorySize(), params.getRuntimeStack());
  enclp.runtime_vaddr = (unsigned long) runtimeFile->getMinVaddr();
  enclp.user_vaddr = (unsigned long) enclaveFile->getMinVaddr();

  runtime_stk_sz = params.getRuntimeStack();
  enclave_stk_sz = params.getEnclaveStack();
  untrusted_size = params.getUntrustedSize();
  untrusted_start = params.getUntrustedMem();

  /* Pass in pages to map to enclave here. */

  int ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);

  if (ret) {
    ERROR("failed to create enclave - ioctl() failed: %d", ret);
    destroy();
    return KEYSTONE_ERROR;
  }

  eid = enclp.eid;

  if(loadELF(runtimeFile) != KEYSTONE_SUCCESS) {
    ERROR("failed to load runtime ELF");
    destroy();
    return KEYSTONE_ERROR;
  }

  if(loadELF(enclaveFile) != KEYSTONE_SUCCESS) {
    ERROR("failed to load enclave ELF");
    destroy();
    return KEYSTONE_ERROR;
  }

  /* initialize stack.
   * this will be deprecated with freemem support */
  initStack(-1UL, runtime_stk_sz, 1);
  initStack(enclave_stk_start, enclave_stk_sz, 0);

  ret = ioctl(fd, KEYSTONE_IOC_UTM_INIT, &enclp);

  if (ret) {
    ERROR("failed to init untrusted memory - ioctl() failed: %d", ret);
    destroy();
    return KEYSTONE_ERROR;
  }

  loadUntrusted();

  ret = ioctl(fd, KEYSTONE_IOC_FINALIZE_ENCLAVE, &enclp);

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
    int ret = ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, &enclp);

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
  struct keystone_ioctl_run_enclave run;
  run.eid = eid;

  ret = ioctl(fd, KEYSTONE_IOC_RUN_ENCLAVE, &run);
  while (ret == KEYSTONE_ENCLAVE_EDGE_CALL_HOST) {
    /* enclave is stopped in the middle. */
    if (oFuncDispatch != NULL) {
      oFuncDispatch(getSharedBuffer());
    }
    ret = ioctl(fd, KEYSTONE_IOC_RESUME_ENCLAVE, &run);
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

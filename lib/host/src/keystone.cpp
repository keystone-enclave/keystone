//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/mman.h>
#include <linux/elf.h>
#include <keystone_user.h>
#include "keystone.h"
#include "keystone_user.h"
#include <math.h>

#define PAGE_SIZE 4096

Keystone::Keystone() {
    runtimeFile = NULL;
    enclaveFile = NULL;
    eid = -1;
}

Keystone::~Keystone() {
    delete runtimeFile;
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

    // FIXME: calculate the required number of pages for the page table.
    // For now, we must allocate at least 1 (top) + 2 (enclave) + 2 (runtime) pages for pg tables
    req_pages += 15;
    return req_pages;
}

struct mapped_meta *Keystone::keystone_rtld_init_rt_stk(vaddr_t stack_addr, unsigned long size, int *error) {

    int i, old_error = *error;
    vaddr_t va_start = ((int) (stack_addr - (size - 1))) / PAGE_SIZE;
    vaddr_t va;
    vaddr_t status;
    //vaddr_t va_end = PAGE_UP(stack_addr - 1);

    struct mapped_meta *meta = (struct mapped_meta *) malloc(sizeof(mapped_meta));
    struct addr_packed *va_pages = (addr_packed *) malloc(sizeof(struct addr_packed) * (size >> 12));

    *error = -ENOMEM;
    if (!meta || !va_pages) {
        return NULL;
    }

    //pr_info("[rt stack] va_start: 0x%lx, va_end: 0x%lx\n", va_start, va_end);

    for (i = 0, va = va_start; i < (int) (size >> 12); i++, va += PAGE_SIZE) {
        //pr_info("mapping: %lx\n",va);

        status = (vaddr_t) mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        va_pages[i].copied = status;
        va_pages[i].va = va;

        if ((int) va_pages[i].copied == -1)
            return NULL;

    }

    *error = old_error;
    meta->num_pages = i;
    meta->meta = va_pages;
    meta->mode = RT_NOEXEC;
//    printf("num_pages: %d\n", meta->num_pages);
    return meta;
}

struct mapped_meta *Keystone::keystone_rtld_init_app_stk(size_t app_stack_sz, unsigned long stack_offset, int *error) {
    unsigned long vaddr;
    int i, old_error = *error;

    /* We pass in -1UL for the stack address, since stack occupies the highest address
     *
     * */

//    int num_pages = stack_offset - app_stack_sz / (PAGE_SIZE) + (PAGE_SIZE);
    int num_pages = ceil(app_stack_sz / (PAGE_SIZE));
    *error = -ENOMEM;
    struct mapped_meta *meta = (struct mapped_meta *) malloc(sizeof(mapped_meta));
    struct addr_packed *va_pages = (addr_packed *) malloc(sizeof(addr_packed) * num_pages);

    if (!meta || !va_pages) {
        return NULL;
    }
//    printf("app_stack_sz: %lu\n", app_stack_sz);
//    printf("stackoffset: %lu\n", stack_offset);
//    printf("num_pages: %d\n", num_pages);

    /* setup enclave stack */
    for (vaddr = stack_offset - app_stack_sz , i = 0;
         vaddr < stack_offset;
         vaddr += PAGE_SIZE, i++) {

        va_pages[i].copied = (vaddr_t) mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        va_pages[i].va = vaddr;

//        printf("copied: %lu, vaddr: %lu, i: %d, n: %d \n", va_pages[i].copied, va_pages[i].va, i, num_pages);

        if ((int) va_pages[i].copied == -1)
            return NULL;
    }
//    printf("in app stk\n");

    // TODO fix eapp_sz so that its smaller, more accurate. right now its the whole elf

    *error = old_error;
    meta->num_pages = i;
    meta->meta = va_pages;
    meta->mode = USER_NOEXEC;
    return meta;
}


struct mapped_meta *Keystone::keystone_app_load_elf_section_NOBITS(void *target_vaddr, size_t len, int *error) {
    vaddr_t va;
    size_t _size;
    unsigned int k;
    int old_error = *error;
    vaddr_t status;

    int num_pages = len / (PAGE_SIZE);

    *error = -ENOMEM;
    struct mapped_meta *meta = (struct mapped_meta *) malloc(sizeof(mapped_meta));
    struct addr_packed *va_pages = (addr_packed *) malloc(sizeof(addr_packed) * num_pages);

    if (!meta || !va_pages) {
        return NULL;
    }

    for (va = (uintptr_t) target_vaddr, k = 0; va < (uintptr_t) target_vaddr + len; va += PAGE_SIZE, k++) {

        status = (vaddr_t) mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);


        //Page wasn't allocated
        if ((int) status == -1) {
            return NULL;
        }

        va_pages[k].copied = (vaddr_t) status;
        va_pages[k].va = va;
        _size = (k + 1) * PAGE_SIZE > len ? len % PAGE_SIZE : PAGE_SIZE;
        memset((void *) va_pages[k].copied, 0, _size);
    }

    *error = old_error;
    meta->num_pages = k;
    meta->meta = va_pages;
    meta->mode = USER_FULL;
    return meta;
}

struct mapped_meta *Keystone::keystone_app_load_elf_region(vaddr_t elf_usr_region,
                                                           void *target_vaddr, size_t len, int *error) {
    vaddr_t va;
    unsigned int k;
    int old_error = *error;
    size_t copy_size;
    int num_pages = len / (PAGE_SIZE);

    *error = -ENOMEM;

    struct mapped_meta *meta = (struct mapped_meta *) malloc(sizeof(mapped_meta));
    struct addr_packed *va_pages = (addr_packed *) malloc(sizeof(addr_packed) * num_pages);

    if (!meta || !va_pages) {

        return NULL;
    }

    for (va = (uintptr_t) target_vaddr, k = 0; va < (uintptr_t) target_vaddr + len; va += PAGE_SIZE, k++) {

        va_pages[k].copied = (vaddr_t) mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC,
                                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        va_pages[k].va = va;

        copy_size = (k + 1) * PAGE_SIZE > len ? len % PAGE_SIZE : PAGE_SIZE;
        //pr_info("Copy elf page to:%x, from: %x, sz:%i\n",
        //	    encl_page, elf_usr_region + (k * PAGE_SIZE), copy_size);
        // TODO zero out the other part of the last page
        memcpy((void *) va_pages[k].copied,
               (void *) (elf_usr_region + (k * PAGE_SIZE)),
               copy_size);
    }

    *error = old_error;
    meta->num_pages = k;
    meta->meta = va_pages;
    meta->mode = USER_FULL;
    //TODO: Free the other allocated stuff
    return meta;
}


struct mapped_meta *Keystone::rtld_vm_mmap(vaddr_t encl_addr, unsigned long size,
                                           vaddr_t rt_ptr, struct elf64_phdr *phdr, int *error) {
    /* Copies program header to address.
     * Return a list of va and copy address pair
     * VA is the point where we want to point to the copy data memory
     * Copy pointer contains pointer to "copy" pages
     *  - Copy pages = temporary memory to hold data
     * We eventually want to copy all the "copy" pages to enclave pages,
     * then have the virtual pointer binded to the enclave pages
     *
     *
     * */
    unsigned int k;
    int old_error = *error;
    vaddr_t va_start = ((int) encl_addr) / (PAGE_SIZE);
    vaddr_t va_end = ((int) (encl_addr + size)) / (PAGE_SIZE) + (PAGE_SIZE);
    int num_pages = (va_start - va_end) / (PAGE_SIZE);
    vaddr_t va;
    vaddr_t status;

    struct mapped_meta *meta = (struct mapped_meta *) malloc(sizeof(mapped_meta));
    struct addr_packed *va_pages = (addr_packed *) malloc(sizeof(addr_packed) * num_pages);
    //pr_info("va_start: 0x%lx, va_end: 0x%lx\n", va_start, va_end);



    vaddr_t pos = phdr->p_offset;
    for (va = va_start, k = 0; va < va_end; va += PAGE_SIZE, k++) {

        status = (vaddr_t) mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (!status) {
            ERROR("Could not allocate userspace memory!");
            *error = -ENOMEM;
            goto out;
        }

        memcpy((void *) status, (void *) (rt_ptr + pos), PAGE_SIZE);

        va_pages[k].copied = status;
        va_pages[k].va = va;

        pos += PAGE_SIZE;

        //debug_dump(epm_page, PAGE_SIZE);
    }
    *error = old_error;
    meta->num_pages = k;
    meta->meta = va_pages;
    meta->mode = RT_FULL;
    out:
    return meta;
}


struct mapped_meta **Keystone::keystone_rtld_init_app(vaddr_t elf_usr_ptr, int *num_pages, int *error) {
    int i, j;
    struct elf64_phdr elf_phdr_tmp;
    struct elf64_shdr elf_shdr_tmp;
    struct elf64_hdr elf_ex;
    struct elf64_phdr *next_usr_phoff;
    struct elf64_shdr *next_usr_shoff;
    unsigned long vaddr;
    unsigned long size = 0;
    struct mapped_meta *status;
    int index;

    *error = -EFAULT;

    // TODO safety checks based on len

    memcpy(&elf_ex, (void *) elf_usr_ptr, sizeof(struct elf64_hdr));

    // check ELF header
    if (memcmp(elf_ex.e_ident, ELFMAG, SELFMAG) != 0)
        return NULL;

    // Sanity check on elf type that its been linked as EXEC
    if (elf_ex.e_type != ET_EXEC) // || !elf_check_arch(&elf_ex))
        return NULL;

    *error = -ENOMEM;

    struct mapped_meta **meta_master = (struct mapped_meta **) malloc(sizeof(mapped_meta *) * (elf_ex.e_phnum + elf_ex.e_shnum));
    struct addr_packed *va_page_list = (struct addr_packed *) calloc(elf_ex.e_phnum + elf_ex.e_shnum,
                                                                     sizeof(struct addr_packed));

    if (!meta_master || !va_page_list) {
        return NULL;
    }

    *error = -EFAULT;

//    printf("elf_ex.e_shoff: %d\n", elf_ex.e_shoff);

    //Iterator for section headers
    next_usr_shoff = (struct elf64_shdr *) ((uintptr_t) elf_usr_ptr + elf_ex.e_shoff);

    /* Allocate pages for each section header
     *
     * */

    printf("elf_ex.e_shnum: %d\n", elf_ex.e_shnum);
    for (i = 0; i < elf_ex.e_shnum; i++, next_usr_shoff++) {

        memcpy(&elf_shdr_tmp, (void *) next_usr_shoff, sizeof(struct elf64_shdr));

        vaddr = elf_shdr_tmp.sh_addr;

        printf("vaddr: %lu\n", vaddr);
//        printf("next_usr_shoff->sh_add: %u\n", next_usr_shoff->sh_addr);
        printf("elf_shdr_tmp.sh_type: %u\n", elf_shdr_tmp.sh_type);

        // Sections with a load address of 0 aren't supposed to be created at runtime
        if (vaddr == 0) {
            continue;
        }

        // We are only handling SHT_NOBITS right now, deal with other types later
        if (elf_shdr_tmp.sh_type != SHT_NOBITS) {
            if (elf_shdr_tmp.sh_type != SHT_PROGBITS) { // Should get handled by phdr loading below
//                pr_warn("Keystone unable to load sections that are not SHT_NOBITS, ignoring (@ 0x%llx)\n",
//                        elf_shdr_tmp.sh_addr);
            }
            continue;
        }

        size = elf_shdr_tmp.sh_size;

//        printf("Before print\n");
        // Create section and set to 0
        status = keystone_app_load_elf_section_NOBITS((void *) vaddr,
                                                      size, error);
        if (*error) {
            return NULL;
        }

        meta_master[i] = status;

        printf("status->num_pages:%d\n",status->num_pages);
        index += 1;

    }
//
//    /* Allocate pages for each program header
//     * phoff = offset to program header, relative to  elf_usr_ptr
//     * p_vaddr = first byte in virtual address space for program header
//     * p_filesizes = bytes of program segment
//     */
//    // Get each elf_phdr in order and deal with it
    printf("elf_ex.e_phnum: %d\n", elf_ex.e_phnum);
    next_usr_phoff = (struct elf64_phdr *) ((vaddr_t) elf_usr_ptr + elf_ex.e_phoff);
    for (j = 0; i < elf_ex.e_phnum; i++, j++, next_usr_phoff++) {


        memcpy(&elf_phdr_tmp, (void *) next_usr_phoff, sizeof(struct elf64_phdr));

        // Create and copy
        if (elf_phdr_tmp.p_type != PT_LOAD) {
//            pr_warn("keystone runtime includes an inconsistent program header\n");
            continue;
        }
        vaddr = elf_phdr_tmp.p_vaddr;
        //vaddr sanity check?
        size = elf_phdr_tmp.p_filesz;
        //pr_info("loading vaddr: %x, sz:%i\n",vaddr,size);

        status = keystone_app_load_elf_region(elf_usr_ptr + elf_phdr_tmp.p_offset,
                                              (void *) vaddr,
                                              size, error);

        printf("status->num_pages:%d\n",status->num_pages);
        if (*error) {
            return NULL;
        }
        meta_master[i] = status;
        index += 1;
    }

    *error = 0;
    *num_pages = index;
    return meta_master;
}


struct mapped_meta **
Keystone::keystone_rtld_init_runtime(vaddr_t rt_ptr, size_t rt_sz, unsigned long *rt_offset, int *num_rt_pages,
                                     int *error) {

    int i;
    struct elf64_phdr *elf_phdata;
    struct elf64_phdr *eppnt;
    struct elf64_hdr elf_ex;
    *rt_offset = -1UL;

    struct mapped_meta **meta_master;

    *error = -ENOEXEC;

    //Reads in ELF header
    elf_ex = *((struct elf64_hdr *) rt_ptr);

    // check ELF header
    if (memcmp(elf_ex.e_ident, ELFMAG, SELFMAG) != 0) {
//        PERROR("invalid runtime executable\n");
        goto out;
    }

    // check runtime consistency
    if (elf_ex.e_type != ET_EXEC) { // || !elf_check_arch(&elf_ex)) {
        //    keystone_err("invalid runtime executable\n");
        goto out;
    }

    *error = -ENOMEM;
//    struct mapped_meta **meta_master = (struct mapped_meta **) malloc(sizeof(mapped_meta *));
    meta_master = (struct mapped_meta **) calloc(elf_ex.e_phnum,
                                                 sizeof(struct mapped_meta *));
    elf_phdata = (struct elf64_phdr *) malloc(sizeof(struct elf64_phdr) * elf_ex.e_phnum);
    //We want to save the original pointer to free later
    eppnt = elf_phdata;
    if (!elf_phdata || !meta_master)
        goto out;

    *error = -ENOEXEC;

    printf("eppnt: %p\n", eppnt);
    printf("elf_ex.e_phnum: %d\n", elf_ex.e_phnum);
    //Writes program header table to eppnt
    memcpy((void *) eppnt, (void *) (rt_ptr + elf_ex.e_phoff), sizeof(struct elf64_phdr) * elf_ex.e_phnum);



    for (eppnt = elf_phdata, i = 0; i < elf_ex.e_phnum; eppnt++, i++) {
        /*
         * Looping through segments
         * eppnt = current segment
         * eppnt -> p_type = type of segment
         * eppnt -> p_vaddr = VA of first byte of first segment
         * elf_ex.e_phnum = number of segments
         *
         * */
        unsigned long vaddr;
        unsigned long size = 0;

        if (eppnt->p_type != PT_LOAD) {
//      keystone_warn("keystone runtime includes an inconsistent program header\n");
            continue;
        }
        vaddr = eppnt->p_vaddr;
        if (vaddr < *rt_offset) {
            *rt_offset = vaddr;
        }
        size = eppnt->p_filesz;
        if (size > eppnt->p_memsz) {
//      pr_info("unexpected mismatch in elf program header: filesz %ld, memsz %llu\n", size, eppnt->p_memsz);
        }

        meta_master[i] = rtld_vm_mmap(vaddr, size, rt_ptr, eppnt, error);

        if (*error) {
            goto out;
        }
    }

    *error = 0;
    free(elf_phdata);
    *num_rt_pages = i;
    out:
    return meta_master;

}

keystone_status_t Keystone::init(const char *eapppath, const char *runtimepath, Params params) {
    if (runtimeFile || enclaveFile) {
        ERROR("ELF files already initialized");
        return KEYSTONE_ERROR;
    }

    runtimeFile = new ELFFile(runtimepath);
    enclaveFile = new ELFFile(eapppath);

    /* these should be parsed by ELF lib */

    /* open device driver */
    fd = open(KEYSTONE_DEV_PATH, O_RDWR);
    if (fd < 0) {
        PERROR("cannot open device file");
        return KEYSTONE_ERROR;
    }

    if (!runtimeFile->isValid()) {
        ERROR("runtime file is not valid");
        return KEYSTONE_ERROR;
    }
    if (!enclaveFile->isValid()) {
        ERROR("enclave file is not valid");
        return KEYSTONE_ERROR;
    }

    /* Call Keystone Driver */
    struct keystone_ioctl_create_enclave enclp;


    //Loads enclave and runtime
//  enclaveFile->getPtr() = (unsigned long) enclaveFile->getPtr();
//  enclp.user_elf_size = (unsigned long) enclaveFile->getSize();
//  runtimeFile->getPtr() = (unsigned long) runtimeFile->getPtr();
//  runtimeFile->getSize() = (unsigned long) runtimeFile->getSize();
//
//  params.getEnclaveStack() = (unsigned long) params.getEnclaveStack();
//  params.getRuntimeStack() = (unsigned long) params.getRuntimeStack();

    enclp.params.runtime_entry = (unsigned long) params.getRuntimeEntry();
    enclp.params.user_entry = (unsigned long) params.getEnclaveEntry();
    enclp.params.untrusted_ptr = (unsigned long) params.getUntrustedMem();
    enclp.params.untrusted_size = (unsigned long) params.getUntrustedSize();

    enclp.min_pages = calculate_required_pages(enclaveFile->getSize(), params.getEnclaveStack(),
                                               runtimeFile->getSize(), params.getRuntimeStack());

    /* argument validity */
    if (!enclaveFile->getPtr() || !runtimeFile->getPtr() || !enclaveFile->getSize() || !runtimeFile->getSize()) {
        ERROR("Arguments are invalid, check again...");
        return KEYSTONE_ERROR;
    }

    unsigned long rt_offset;
    int runtime_num_headers;
    int app_num_headers;
    int error;
    int i, j;
    int status;

    /* Pass in pages to map to enclave here. */

    int ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);

    if (ret) {
        ERROR("failed to create enclave - ioctl() failed: %d", ret);
        return KEYSTONE_ERROR;
    }
    printf("Loading rt stk..\n");

    struct mapped_meta *rt_stk = keystone_rtld_init_rt_stk(-1UL, params.getRuntimeStack() / (PAGE_SIZE) + (PAGE_SIZE),
                                                           &error);

    printf("rt stk num_pages: %d\n", rt_stk->num_pages);
    for (i = 0; i < rt_stk->num_pages; i++) {
        rt_stk->meta[i].eid = enclp.eid;
        rt_stk->meta[i].mode = RT_NOEXEC;
        printf("va: %lu", rt_stk->meta[i].va);
        printf("copied: %lu", rt_stk->meta[i].copied);
        status = ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &rt_stk->meta[i]);
        if (status) {
            ERROR("failed to create enclave - ioctl() failed: %d", status);
            return KEYSTONE_ERROR;
        }
        munmap((void *) rt_stk->meta[i].copied, PAGE_SIZE);
    }


    printf("Loading rt app..\n");

    struct mapped_meta **rt_prog = keystone_rtld_init_runtime((vaddr_t) runtimeFile->getPtr(), runtimeFile->getSize(),
                                                              &rt_offset,
                                                              &runtime_num_headers, &error);

    printf("runtime_num_headers: %d\n", runtime_num_headers);

    for (i = 0; i < runtime_num_headers; i++) {
        for (j = 0; j < rt_prog[i]->num_pages; j++) {
            rt_prog[i]->meta[j].eid = enclp.eid;
            rt_prog[i]->meta[j].mode = RT_FULL;

            status = ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &rt_prog[i]->meta[j]);
            if (status) {
                ERROR("failed to create enclave - ioctl() failed: %d", status);
                return KEYSTONE_ERROR;
            }
            munmap((void *) rt_prog[i]->meta[j].copied, PAGE_SIZE);
        }
    }

    printf("Loading user stk..\n");

    struct mapped_meta *app_stk = keystone_rtld_init_app_stk(params.getEnclaveStack(), rt_offset, &error);

    printf("app_stk->num_pages: %d\n", app_stk->num_pages);
    for (i = 0; i < app_stk->num_pages; i++) {
        app_stk->meta[i].eid = enclp.eid;
        app_stk->meta[i].mode = USER_NOEXEC;
        status = ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &app_stk->meta[i]);
        if (status) {
            ERROR("failed to create enclave - ioctl() failed: %d", status);
            return KEYSTONE_ERROR;
        }
        munmap((void *) app_stk->meta[i].copied, PAGE_SIZE);
    }

    printf("Loading user app..\n");
    printf("enclavePtr: %p, runtimefile size: %lu, app_num_Headers: %p\n", enclaveFile->getPtr(), runtimeFile->getSize(), &app_num_headers);

    struct mapped_meta **app_prog = keystone_rtld_init_app((vaddr_t) enclaveFile->getPtr(), &app_num_headers, &error);

    printf("app_num_headers: %d\n", app_num_headers);

    for (i = 0; i < app_num_headers; i++) {
        for (j = 0; j < app_prog[i]->num_pages; j++) {
            app_prog[i]->meta[j].eid = enclp.eid;
            app_prog[i]->meta[j].mode = USER_FULL;
            status = ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &app_prog[i]->meta[j]);
            if (status) {
                ERROR("failed to create enclave - ioctl() failed: %d", status);
                return KEYSTONE_ERROR;
            }
            munmap((void *) app_prog[i]->meta[j].copied, PAGE_SIZE);
        }
    }

    printf("DONE w/ ELF loading\n");

    eid = enclp.eid;

    printf("untrusted_size BEFORE FINALIZE: %llu\n", enclp.params.untrusted_size);
    printf("unntrusted_ptr BEFORE FINALIZE: %llu\n", enclp.params.untrusted_ptr);

    ret = ioctl(fd, KEYSTONE_IOC_FINALIZE_ENCLAVE, &enclp);

    if (ret) {
        ERROR("failed to finalize enclave - ioctl() failed: %d", ret);
        return KEYSTONE_ERROR;
    }

    /* Files were consumed by driver and copied into epm, no longer needed */
    delete enclaveFile;
    delete runtimeFile;


    return mapUntrusted(params.getUntrustedSize());
}

keystone_status_t Keystone::mapUntrusted(size_t size) {
    shared_buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shared_buffer == NULL) {
        return KEYSTONE_ERROR;
    }
    shared_buffer_size = size;

    return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::destroy() {
    struct keystone_ioctl_create_enclave enclp;
    enclp.eid = eid;
    int ret = ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, &enclp);

    if (ret) {
        ERROR("failed to destroy enclave - ioctl() failed: %d", ret);
        return KEYSTONE_ERROR;
    }

    return KEYSTONE_SUCCESS;
}

#define KEYSTONE_ENCLAVE_EDGE_CALL_HOST  11

keystone_status_t Keystone::run() {
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

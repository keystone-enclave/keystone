//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/mman.h>
#include "elf.h"
//#include <linux/elf.h>
#include <keystone_user.h>
#include "keystone.h"
#include "keystone_user.h"
#include <math.h>

static char nullpage[PAGE_SIZE] = {0,};

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


keystone_status_t Keystone::loadApp(void) {
    vaddr_t min_vaddr, max_vaddr;
    elf_t elf;
    if (elf_newFile(enclaveFile->getPtr(), enclaveFile->getSize(), &elf)) {
        ERROR("Invalid Enclave ELF\n");
        return KEYSTONE_ERROR;
    }


    vaddr_t high_addr = enclave_stk_start;
    vaddr_t va_start_stk = ROUND_DOWN((high_addr - enclave_stk_sz - 1), PAGE_BITS);
    int stk_pages = (high_addr - va_start_stk) / PAGE_SIZE;


    for (int i = 0; i < stk_pages; i++) {
        struct addr_packed encl_page;
        encl_page.copied = (vaddr_t) nullpage;
        encl_page.va = va_start_stk;
        encl_page.eid = eid;
        encl_page.mode = USER_NOEXEC;

        if (ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &encl_page)) {
            PERROR("failed to add page - ioctl() failed");
        }
        va_start_stk += PAGE_SIZE;
    }


    elf_getMemoryBounds(&elf, VIRTUAL, &min_vaddr, &max_vaddr);

    max_vaddr = ROUND_UP(max_vaddr, PAGE_BITS);


    for (unsigned int i = 0; i < elf_getNumProgramHeaders(&elf); i++) {

        if (elf_getProgramHeaderType(&elf, i) != PT_LOAD) {
            printf("inconsistent header\n");
            printf("segment: 0x%lx, size: 0x%lx\n", elf_getProgramHeaderVaddr(&elf, i),
                   elf_getProgramHeaderFileSize(&elf, i));
            continue;
        }

        vaddr_t start = elf_getProgramHeaderVaddr(&elf, i);
        vaddr_t end = start + elf_getProgramHeaderFileSize(&elf, i);
        vaddr_t src = (vaddr_t) elf_getProgramSegment(&elf, i);
        vaddr_t va;


        for (va = start; va < end; va += PAGE_SIZE) {
            struct addr_packed encl_page;
            encl_page.copied = src;
            encl_page.va = va;
            encl_page.eid = eid;
            encl_page.mode = USER_FULL;

            if (ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &encl_page)) {
                PERROR("failed to add page - ioctl() failed");
            }

            src += PAGE_SIZE;
        }

        /* pad .bss segment */
        while (va < max_vaddr) {
            struct addr_packed encl_page;
            encl_page.copied = (vaddr_t) nullpage;
            encl_page.va = va;
            encl_page.eid = eid;
            encl_page.mode = USER_FULL;

            if (ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &encl_page)) {
                PERROR("failed to add page - ioctl() failed");
            }

            va += PAGE_SIZE;
        }
    }

    return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::loadRuntime(void) {
    vaddr_t min_vaddr, max_vaddr;
    elf_t elf;
    vaddr_t va;


    vaddr_t high_addr = -1UL;
    vaddr_t va_start_stk = ROUND_DOWN((high_addr - runtime_stk_sz - 1), PAGE_BITS);

    int stk_pages = (high_addr - va_start_stk) / PAGE_SIZE;


    for (int i = 0; i <= stk_pages; i++) {
        struct addr_packed encl_page;
        encl_page.copied = (vaddr_t) nullpage;
        encl_page.va = va_start_stk;
        encl_page.eid = eid;
        encl_page.mode = RT_NOEXEC;
        if (ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &encl_page)) {
            PERROR("failed to add page - ioctl() failed");
        }

        va_start_stk += PAGE_SIZE;

    }

    if (elf_newFile(runtimeFile->getPtr(), runtimeFile->getSize(), &elf)) {
        ERROR("Invalid Runtime ELF\n");
        return KEYSTONE_ERROR;
    }

    elf_getMemoryBounds(&elf, VIRTUAL, &min_vaddr, &max_vaddr);
    max_vaddr = ROUND_UP(max_vaddr, PAGE_BITS);

    for (unsigned int i = 0; i < elf_getNumProgramHeaders(&elf); i++) {

        if (elf_getProgramHeaderType(&elf, i) != PT_LOAD) {
            printf("inconsistent header\n");
            printf("segment: 0x%lx, size: 0x%lx\n", elf_getProgramHeaderVaddr(&elf, i),
                   elf_getProgramHeaderFileSize(&elf, i));
            continue;
        }

        vaddr_t start = elf_getProgramHeaderVaddr(&elf, i);
        vaddr_t end = start + elf_getProgramHeaderFileSize(&elf, i);
        vaddr_t src = (vaddr_t) elf_getProgramSegment(&elf, i);

        for (va = start; va < end; va += PAGE_SIZE) {
            struct addr_packed encl_page;
            encl_page.copied = src;
            encl_page.va = va;
            encl_page.eid = eid;
            encl_page.mode = RT_FULL;

            if (ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &encl_page)) {
                PERROR("failed to add page - ioctl() failed");
            }

            src += PAGE_SIZE;
        }


        /* pad .bss segment */
        while (va < max_vaddr) {
            struct addr_packed encl_page;
            encl_page.copied = (vaddr_t) nullpage;
            encl_page.va = va;
            encl_page.eid = eid;
            encl_page.mode = RT_FULL;

            if (ioctl(fd, KEYSTONE_IOC_ADD_PAGE, &encl_page)) {
                PERROR("failed to add page - ioctl() failed");
            }

            va += PAGE_SIZE;
        }
    }

    enclave_stk_start = min_vaddr;

    return KEYSTONE_SUCCESS;
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

    runtime_stk_sz = params.getRuntimeStack();
    enclave_stk_sz = params.getEnclaveStack();
    untrusted_size = params.getUntrustedSize();
    untrusted_start = params.getUntrustedMem();

    /* Pass in pages to map to enclave here. */

    int ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);

    if (ret) {
        ERROR("failed to create enclave - ioctl() failed: %d", ret);
        return KEYSTONE_ERROR;
    }

    eid = enclp.eid;

    // init runtime 
    loadRuntime();
    loadApp();

    ret = ioctl(fd, KEYSTONE_IOC_UTM_INIT, &enclp);

    if (ret) {
        ERROR("failed to init untrusted memory - ioctl() failed: %d", ret);
        return KEYSTONE_ERROR;
    }

    loadUntrusted();

    eid = enclp.eid;


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

//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "keystone.h"
#include "keystone-sbi-arg.h"
#include "keystone_user.h"
#include <linux/uaccess.h>


int keystone_create_enclave(struct file *filp, unsigned long arg) {
    int ret = 0;

    pr_info("Creating enclave..");
    /* create parameters */
    struct keystone_ioctl_create_enclave *enclp = (struct keystone_ioctl_create_enclave*) arg;
    /* What needs to be done in kernel?
     *  1) Create enclave
     *  2) Allocate memory for each segment in program header and setup stack
     *  3)
     *
     * */

    enclave_t *enclave;
//    pr_info("arg_ptr: %px\n", enclp);
//    pr_info("kernel min_pages: %llu\n", enclp->min_pages);
    //Required to be in kernel to create enclave page
    enclave = create_enclave(enclp->min_pages);
    if (enclave == NULL) {
        ret = -ENOMEM;
        goto error_no_free;
    }

    /* allocate UID */
    enclp->eid = enclave_idr_alloc(enclave);

    error_no_free:
    return ret;
}


int keystone_finalize_enclave(struct file *filp, unsigned long arg){

    pr_info("Finalizing...\n");
    int ret;
    enclave_t *enclave;
    struct utm_t* utm;

    struct keystone_ioctl_create_enclave *enclp = (struct keystone_ioctl_create_enclave*) arg;

    // Untrusted mmap size
    void* untrusted_ptr = (void*) enclp->params.untrusted_ptr;
    long long unsigned untrusted_size = enclp->params.untrusted_size;

    enclave = get_enclave_by_id(enclp->eid);

    /* initialize runtime */
    ret = -EFAULT;

    /* Untrusted Memory
     * Shared memory between host and enclave(?)
     * Probably only done in kernel-space (only the kernel can interact w/ SM)
     * */

    utm = kmalloc(sizeof(struct utm_t), GFP_KERNEL);
    if (!utm) {
        ret = -ENOMEM;
        pr_info("Failed utm kalloc...\n");
        goto error_destroy_enclave;
    }


    ret = utm_init(utm, untrusted_size);

    if (ret != 0){
        pr_info("Failed to init utm...\n");
        goto error_destroy_enclave;
    }


    filp->private_data = utm;
    enclave->utm = utm;

//    pr_info("filp: %px\n", filp);
//    pr_info("filp->private_data: %px\n", filp->private_data);
//    pr_info("enclave;: %px\n", enclave);
//    pr_info("enclave->utm: %px\n", enclave->utm);
//    pr_info("untrusted_size: %llu\n", untrusted_size);
//    pr_info("untrusted_ptr: %px\n", untrusted_ptr);


    if (keystone_rtld_init_untrusted(enclave, untrusted_ptr, untrusted_size)) {
        keystone_err("failed to initialize untrusted memory\n");
        goto error_destroy_enclave;
    }

    /* End untrusted memory allocation */

    // runtime parameters
    struct keystone_sbi_create_t create_args;

    /* SBI Call */

    create_args.epm_region.paddr = enclave->epm->pa;
    create_args.epm_region.size = enclave->epm->total;
    create_args.utm_region.paddr = __pa(utm->ptr);
    create_args.utm_region.size = utm->size;

    create_args.params = enclp->params;

    // SM will write the eid to enclave_t.eid
    create_args.eid_pptr = (unsigned int *) __pa(&enclave->eid);

    pr_info("create_args.epm_region.paddr: 0x%px\n", create_args.epm_region.paddr);
    pr_info("create_args.epm_region.size: %d\n", create_args.epm_region.size);
    pr_info("create_args.utm_region.paddr: 0x%px\n", create_args.utm_region.paddr);
    pr_info("create_args.utm_region.size: %d\n", create_args.utm_region.size);

    ret = SBI_CALL_1(SBI_SM_CREATE_ENCLAVE, __pa(&create_args));
    if (ret) {
        keystone_err("keystone_create_enclave: SBI call failed\n");
        goto error_destroy_enclave;
    }

    pr_info("Finished SBI call");

    /* We cleanup the free lists here since the kernel will no longer be
       managing them, they are part of the enclave now. */
    utm_clean_free_list(utm);
    epm_clean_free_list(enclave->epm);

    return 0;

    error_destroy_enclave:
    /* This can handle partial initialization failure */
    destroy_enclave(enclave);

    return ret;


}


int keystone_add_page(unsigned long arg){
    int ret = 0;
    struct addr_packed *addr = (struct addr_packed *) arg;
    unsigned long ueid = addr->eid;
    unsigned int mode = addr->mode;
    enclave_t *enclave;
    enclave = get_enclave_by_id(ueid);
    vaddr_t epm_page;
    switch(mode){
        case RT_NOEXEC:
            epm_alloc_rt_page_noexec(enclave->epm, addr->va);
            break;
        case USER_NOEXEC:
            epm_alloc_user_page_noexec(enclave->epm, addr->va);
            break;
        case RT_FULL:
            epm_page = epm_alloc_rt_page(enclave->epm, addr->va);
            if(copy_from_user((void*)epm_page, (void *) addr->copied, PAGE_SIZE) != 0)
                ret = -ENOEXEC;
            break;
        case USER_FULL:
            epm_page = epm_alloc_user_page(enclave->epm, addr->va);
            if(copy_from_user((void*)epm_page, (void *) addr->copied, PAGE_SIZE) != 0)
                ret = -ENOEXEC;
            break;
        default:
            ret = -ENOSYS;
    }
    return ret;
}


int keystone_destroy_enclave(struct file *filp, unsigned long arg) {
    int ret;
    struct keystone_ioctl_create_enclave *enclp = (struct keystone_ioctl_create_enclave *) arg;
    unsigned long ueid = enclp->eid;
    enclave_t *enclave;
    enclave = get_enclave_by_id(ueid);

    if (!enclave) {
        keystone_err("invalid enclave id\n");
        return -EINVAL;
    }
    ret = SBI_CALL_1(SBI_SM_DESTROY_ENCLAVE, enclave->eid);
    if (ret) {
        keystone_err("fatal: cannot destroy enclave: SBI failed\n");
        return ret;
    }

    destroy_enclave(enclave);
    enclave_idr_remove(ueid);

    return 0;
}

int keystone_run_enclave(struct file *filp, unsigned long arg) {
    int ret = 0;
    struct keystone_ioctl_run_enclave *run = (struct keystone_ioctl_run_enclave *) arg;
    unsigned long ueid = run->eid;
    enclave_t *enclave;
    enclave = get_enclave_by_id(ueid);

    if (!enclave) {
        keystone_err("invalid enclave id\n");
        return -EINVAL;
    }

    ret = SBI_CALL_1(SBI_SM_RUN_ENCLAVE, enclave->eid);
    /* if the enclave is timer-interrupted, just resume the enclave */
    while (ret == ENCLAVE_INTERRUPTED) {
        keystone_handle_interrupts();
        ret = SBI_CALL_1(SBI_SM_RESUME_ENCLAVE, enclave->eid);
    }

    return ret;
}

int keystone_resume_enclave(struct file *filp, unsigned long arg) {
    int ret = 0;
    struct keystone_ioctl_run_enclave *resume = (struct keystone_ioctl_run_enclave *) arg;
    unsigned long ueid = resume->eid;
    enclave_t *enclave;
    enclave = get_enclave_by_id(ueid);

    if (!enclave) {
        keystone_err("invalid enclave id\n");
        return -EINVAL;
    }

    ret = SBI_CALL_1(SBI_SM_RESUME_ENCLAVE, enclave->eid);
    while (ret == ENCLAVE_INTERRUPTED) {
        keystone_handle_interrupts();
        ret = SBI_CALL_1(SBI_SM_RESUME_ENCLAVE, enclave->eid);
    }

    return ret;
}

long keystone_ioctl(struct file *filep, unsigned int cmd, unsigned long arg) {
    long ret;
    char data[256];
    size_t ioc_size;

//    pr_info("keystone_enclave: keystone_ioctl() command = %d\n",cmd);
//    pr_info("keystone_enclave:arg addr = %px\n", arg);

    if (!arg)
        return -EINVAL;

    ioc_size = _IOC_SIZE(cmd);
//    pr_info("ioc_size:  %d\n", ioc_size);
    ioc_size = ioc_size > sizeof(data) ? sizeof(data) : ioc_size;



    if (copy_from_user(data,(void __user
    *) arg, ioc_size))
    return -EFAULT;

    switch (cmd) {
        case KEYSTONE_IOC_CREATE_ENCLAVE:
            ret = keystone_create_enclave(filep, (unsigned long) data);
            break;
        case KEYSTONE_IOC_ADD_PAGE:
            ret = keystone_add_page((unsigned long) data);
            break;
        case KEYSTONE_IOC_FINALIZE_ENCLAVE:
            ret = keystone_finalize_enclave(filep, (unsigned long) data);
            break;
        case KEYSTONE_IOC_DESTROY_ENCLAVE:
            ret = keystone_destroy_enclave(filep, (unsigned long) data);
            break;
        case KEYSTONE_IOC_RUN_ENCLAVE:
            ret = keystone_run_enclave(filep, (unsigned long) data);
            break;
        case KEYSTONE_IOC_RESUME_ENCLAVE:
            ret = keystone_resume_enclave(filep, (unsigned long) data);
            break;
        default:
            return -ENOSYS;
    }

    if (copy_to_user((void __user
    *) arg, data, ioc_size))
    return -EFAULT;

    return ret;
}



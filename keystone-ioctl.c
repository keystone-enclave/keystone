//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "keystone.h"
#include "keystone-sbi-arg.h"
#include "keystone_user.h"
#include <linux/uaccess.h>


int keystone_create_enclave(unsigned long arg) {
    int ret = 0;

    /* create parameters */
    struct keystone_ioctl_create_enclave *enclp = (struct keystone_ioctl_create_enclave *) arg;

    enclave_t *enclave;
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


int keystone_finalize_enclave(unsigned long arg) {

    int ret;
    enclave_t *enclave;
    struct utm_t *utm;

    struct keystone_ioctl_create_enclave *enclp = (struct keystone_ioctl_create_enclave *) arg;

    enclave = get_enclave_by_id(enclp->eid);
    utm = enclave->utm;


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

    ret = SBI_CALL_1(SBI_SM_CREATE_ENCLAVE, __pa(&create_args));
    if (ret) {
        keystone_err("keystone_create_enclave: SBI call failed\n");
        goto error_destroy_enclave;
    }

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


int keystone_add_page(unsigned long arg) {
    int ret = 0;
    struct addr_packed *addr = (struct addr_packed *) arg;
    unsigned long ueid = addr->eid;
    unsigned int mode = addr->mode;
    enclave_t *enclave;
    enclave = get_enclave_by_id(ueid);
    vaddr_t epm_page;

    switch (mode) {
        case USER_NOEXEC: {
            epm_alloc_user_page_noexec(enclave->epm, addr->va);
            break;
        }
        case RT_NOEXEC: {
            epm_alloc_rt_page_noexec(enclave->epm, addr->va);
            break;
        }
        case RT_FULL: {
            epm_page = epm_alloc_rt_page(enclave->epm, addr->va);
            if (copy_from_user((void *) epm_page, (void *) addr->copied, PAGE_SIZE) != 0)
                ret = -ENOEXEC;
            break;
        }
        case USER_FULL: {
            epm_page = epm_alloc_user_page(enclave->epm, addr->va);
            if (copy_from_user((void *) epm_page, (void *) addr->copied, PAGE_SIZE) != 0)
                ret = -ENOEXEC;
            break;
        }
        default:
            ret = -ENOSYS;
    }
    return ret;
}

int utm_init_ioctl(struct file *filp, unsigned long arg){
    struct utm_t *utm;
    enclave_t *enclave;
    struct keystone_ioctl_create_enclave *enclp = (struct keystone_ioctl_create_enclave *) arg;
    long long unsigned untrusted_size = enclp->params.untrusted_size;
    int ret = 0;
    enclave = get_enclave_by_id(enclp->eid);
    utm = kmalloc(sizeof(struct utm_t), GFP_KERNEL);
    if (!utm) {
        ret = -ENOMEM;
        return ret;
    }


    ret = utm_init(utm, untrusted_size);

    filp->private_data = utm;
    enclave->utm = utm;

    return ret;
}

int utm_alloc(unsigned long arg) {

    int ret = 0;
    struct addr_packed *addr = (struct addr_packed *) arg;
    unsigned long ueid = addr->eid;

    enclave_t *enclave;
    enclave = get_enclave_by_id(ueid);

    utm_alloc_page(enclave->utm, enclave->epm, addr->va, PTE_D | PTE_A | PTE_R | PTE_W);

    return ret;
}


int keystone_destroy_enclave(unsigned long arg) {
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

int keystone_run_enclave(unsigned long arg) {
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

int keystone_resume_enclave(unsigned long arg) {
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

    if (!arg)
        return -EINVAL;

    ioc_size = _IOC_SIZE(cmd);
    ioc_size = ioc_size > sizeof(data) ? sizeof(data) : ioc_size;


    if (copy_from_user(data,(void __user *) arg, ioc_size))
    return -EFAULT;

    switch (cmd) {
        case KEYSTONE_IOC_CREATE_ENCLAVE:
            ret = keystone_create_enclave((unsigned long) data);
            break;
        case KEYSTONE_IOC_ADD_PAGE:
            ret = keystone_add_page((unsigned long) data);
            break;
        case KEYSTONE_IOC_FINALIZE_ENCLAVE:
            ret = keystone_finalize_enclave((unsigned long) data);
            break;
        case KEYSTONE_IOC_DESTROY_ENCLAVE:
            ret = keystone_destroy_enclave((unsigned long) data);
            break;
        case KEYSTONE_IOC_RUN_ENCLAVE:
            ret = keystone_run_enclave((unsigned long) data);
            break;
        case KEYSTONE_IOC_RESUME_ENCLAVE:
            ret = keystone_resume_enclave((unsigned long) data);
            break;
        case KEYSTONE_IOC_UTM_ALLOC:
            ret = utm_alloc((unsigned long) data);
            break;
        case KEYSTONE_IOC_UTM_INIT:
            ret = utm_init_ioctl(filep, (unsigned long) data);
            break;
        default:
            return -ENOSYS;
    }

    if (copy_to_user((void __user
    *) arg, data, ioc_size))
    return -EFAULT;

    return ret;
}



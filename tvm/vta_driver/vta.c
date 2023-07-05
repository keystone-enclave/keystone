#include <linux/clk.h>
#include <linux/device.h>
#include <linux/genalloc.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/uio_driver.h>

MODULE_DESCRIPTION("TVM-VTA Driver for Polarfire");
MODULE_AUTHOR("Gregor Haas <ghaas97@cs.washington.edu>");
MODULE_VERSION("1.0.0");
MODULE_LICENSE("GPL");

struct vta_dev {
    struct uio_info *info;
    void __iomem *base;
};

static int vta_probe(struct platform_device *pdev)
{
    int ret = -EINVAL, len;
    struct uio_info *uio_info;
    struct vta_dev *uio_vta;
    struct resource *regs;

    uio_vta = kzalloc(sizeof(*uio_vta), GFP_KERNEL);
    if(!uio_vta)
        return -ENOMEM;

    uio_vta->info = kzalloc(sizeof(*uio_info), GFP_KERNEL);
    if(!uio_vta->info) {
        ret = -ENOMEM; goto out_free;
    }

    regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(!regs) {
        ret = -ENODEV; goto out_free;
    }

    len = resource_size(regs);
    uio_vta->base = ioremap(regs->start, len);
    if(!uio_vta->base) {
        ret = -ENODEV; goto out_free;
    }

    uio_info = uio_vta->info;
    uio_info->mem[0].addr = regs->start;
    uio_info->mem[0].size = len;
    uio_info->mem[0].memtype = UIO_MEM_PHYS;

    uio_info->mem[1].size = 0;

    uio_info->name = pdev->dev.of_node->full_name;
    uio_info->version = "0.1";

    ret = uio_register_device(&pdev->dev, uio_info);
    if(ret < 0) {
        goto out_free;
    }

    platform_set_drvdata(pdev, uio_vta);
    return 0;

out_free:
    if(uio_vta) {
        if(uio_vta->base) {
            iounmap(uio_vta->base);
        }

        if(uio_vta->info) {
            kfree(uio_vta->info);
        }

        kfree(uio_vta);
    }

    return ret;
}

static int vta_remove(struct platform_device *dev)
{
    struct vta_dev *dev_info = platform_get_drvdata(dev);

    uio_unregister_device(dev_info->info);
    kfree(dev_info->info->name);
    iounmap(dev_info->base);
    kfree(dev_info->info);
    kfree(dev_info);
    return 0;
}

#if defined(CONFIG_OF)
static const struct of_device_id vta_dt_ids[] = {
        { .compatible = "vta-fpga" } ,
        { /* sentinel */ }
};
#endif

static struct platform_driver vta_driver = {
        .probe = vta_probe,
        .remove = vta_remove,
        .driver = {
                .name = "vta",
                .pm = NULL,
                .of_match_table = of_match_ptr(vta_dt_ids),
                .owner = THIS_MODULE
        },
};

module_platform_driver(vta_driver);

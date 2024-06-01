#include <linux/fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include "uaf.h"

#define MISC_DEVICE_NAME "vuln_miscdevice"

MODULE_LICENSE("MIT");

static long handle_ioctl(
    struct file* f, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
    case ALLOC_UAF_OBJ:
        alloc_uaf_obj(arg);
        break;
    case USE_UAF_OBJ:
        use_uaf_obj();
        break;
    case ALLOC_K_OBJ:
        alloc_k_obj((struct KernelObject*)arg);
        break;
    case FREE_UAF_OBJ:
        free_uaf_obj();
        break;
    }

    return 1;
}

static struct file_operations fop
    = {.owner = THIS_MODULE, .unlocked_ioctl = &handle_ioctl };

static struct miscdevice my_miscdevice
    = {.name = MISC_DEVICE_NAME, .fops = &fop };

static int __init uaf_init(void)
{
    int ret = misc_register(&my_miscdevice);

    if (ret < 0) {
        pr_err("misc device registration failed");
        return ret;
    }

    return ret;
}

static void __exit uaf_exit(void) { misc_deregister(&my_miscdevice); }

module_init(uaf_init);
module_exit(uaf_exit);

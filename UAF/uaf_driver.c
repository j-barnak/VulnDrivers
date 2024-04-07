#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

typedef struct uaf_object {
    char uaf_buffer_one[56];
    long args;
    void (*cb)(void);
    char uaf_buffer_two[20];
} uaf_object_t;

typedef struct k_object { char k_buffer[96]; } k_object_t;

void uaf_callback(void);
int uaf_alloc_object(unsigned long __user args);
int uaf_use_object(void);
void uaf_free_object(void);
int alloc_k_object(k_object_t* user_objects);

uaf_object_t* uaf_object_context;

int uaf_alloc_object(unsigned long __user args) {
    uaf_object_t* obj = kmalloc(sizeof(uaf_object_t), GFP_KERNEL);

    if (!obj) {
	printk("Out of memory\n");
	return ENOMEM;
    }

    obj->args = args;
    obj->cb = &uaf_callback;

    uaf_object_context = obj;

    pr_debug("== Allocated uaf_object_t ==");

    return 0;
}

int uaf_use_object() {
    printk("Calling fn_ptr: 0x%p\n", uaf_object_context->cb);
    uaf_object_context->cb();

    return 0;
}

void uaf_free_object() {
    printk("== Freed uaf_object_t ==");
    kfree(uaf_object_context);
}

void uaf_callback(void) { printk("callback entered\n"); }

int alloc_k_object(k_object_t* user_k_object) {
    int ret = 0;
    k_object_t* trash_object = kmalloc(sizeof(k_object_t), GFP_KERNEL);

    if (!trash_object) {
	pr_debug("Error Alloacting Kernel Object");
	return ENOMEM;
    }

    pr_debug("Allocated Kernel Object");

    ret = copy_from_user(trash_object, user_k_object, sizeof(k_object_t));

    return ret;
}


enum IoctlSwitch {
    ALLOC_UAF_OBJECT = 0,
    USE_UAF_OBJECT = 1,
    FREE_UAF_OBJECT = 2,
    ALLOC_K_OBJECT = 3
};

static long ioctl_fn(struct file *f, unsigned int cmd, unsigned long args) {
    switch (cmd) {
	case ALLOC_UAF_OBJECT: {
	    uaf_alloc_object(args);
	    break;
	}

	case USE_UAF_OBJECT: {
	    uaf_use_object();
	    break;
	}

	case FREE_UAF_OBJECT: {
	    uaf_free_object();
	    break;
	}

	case ALLOC_K_OBJECT: {
	    break;
	}

	default:
	    pr_debug("invalid cmd");
    }

    return 0;
}

static const struct file_operations init_fops(void) {
    struct file_operations fop = {.unlocked_ioctl = &ioctl_fn};
    return fop;
}

static struct miscdevice initialize_vuln_misc_driver(void) {
    struct file_operations fop = init_fops();

    struct miscdevice m = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "Vulnerable UAF Driver",
	.fops = &fop,
	.list = {},
	.parent = NULL,
	.this_device = NULL,
	.groups = NULL,
	.nodename = "Awesome UAF Vuln",
	.mode = 1,
    };

    return m;
}

static int __init uaf_init(void) {
    struct miscdevice m = initialize_vuln_misc_driver();
    misc_register(&m);

    return 0;
}

static void __exit uaf_exit(void) { pr_debug("goodbye, exit\n"); }

module_init(uaf_init);
module_exit(uaf_exit);

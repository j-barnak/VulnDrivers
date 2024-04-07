#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

#include "./uaf.h"

enum IoctlSwitch {
	ALLOC_UAF_OBJECT 	= 0,
	USE_UAF_OBJECT 		= 1,
	FREE_UAF_OBJECT		= 2,
	ALLOC_K_OBJECT		= 3
};

static long
ioctl_fn(struct file *f, unsigned int cmd, unsigned long args)
{
	switch(cmd) {
		case ALLOC_UAF_OBJECT:
		{
			uaf_alloc_object(args);
			break;
		}

		case USE_UAF_OBJECT: 
		{
			uaf_use_object();
			break;
	    }

		case FREE_UAF_OBJECT:
		{
			uaf_free_object();
			break;
		}

		case ALLOC_K_OBJECT:
		{
			break;
		}

		default: pr_debug("invalid cmd");
	}

	return 0;
}

static const struct file_operations
init_fops(void)
{
	struct file_operations fop = {
		.unlocked_ioctl = &ioctl_fn
	};

	return fop;
}

static struct miscdevice
initialize_vuln_misc_driver(void)
{
	struct file_operations fop = init_fops();

	struct miscdevice m = { 
		.minor = MISC_DYNAMIC_MINOR, 
		.name  			= "Vulnerable UAF Driver",
		.fops  			= &fop,
		.list 			= {},
		.parent 		= NULL,
		.this_device 	= NULL,
		.groups 	 	= NULL,
		.nodename  		= "Awesome UAF Vuln", 
		.mode  			= 1, 
	};

	return m;
}

static int __init
uaf_init(void) 
{
	struct miscdevice m = initialize_vuln_misc_driver();
	misc_register(&m);	

	return 0;
}

static void __exit
uaf_exit(void) 
{
	pr_debug("goodbye, exit\n");
}

module_init(uaf_init);
module_exit(uaf_exit);

MODULE_LICENSE("GPL");

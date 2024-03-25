#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jared");
MODULE_DESCRIPTION("Vulnerable UAF Driver :)");

typedef struct uaf_object {
	char uaf_buffer_one[56];
	long args;
	void (*fn_pointer)(void);
	char uaf_buffer_two[20];
} uaf_object_t;


uaf_object_t *__uaf_object_context; 

typedef struct k_object {
	char k_buffer[96];
} k_object_t;

static void uaf_callback(void) { printk("callback entered\n"); }

static int
uaf_alloc_obj(unsigned long __user args)
{ 
	uaf_object_t *obj = kmalloc(sizeof(uaf_object_t), GFP_KERNEL);
	if (!obj) { 
		printk("Out of memory\n");
		return ENOMEM;
	}

	obj->args = args;
	obj->fn_pointer = &uaf_callback;

	__uaf_object_context  = obj;

	printk("== Allocated uaf_object_t ==");

	return 0;
}

static int
uaf_use_object() {
	if (!(__uaf_object_context->fn_ptr)) {
		printk("Null")
		return -1;
	}

	printk("Calling fn_ptr: 0x%p\n", __uaf_object_conext->fn_ptr);
	__uaf_object_context->fn_ptr();
}

static void
uaf_free_obj(void)
{ 
	printk("== Freed uaf_object_t ==");
	kfree(__uaf_object_context);
}

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
			uaf_alloc_obj(args);
			break;
		}

		case USE_UAF_OBJECT: 
		{
			break;
	    }

		case FREE_UAF_OBJECT:
		{
			uaf_free_obj();
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

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jared");
MODULE_DESCRIPTION("Vulnerable UAF Driver :)");


static struct miscdevice
initialize_vuln_misc_driver(void)
{
	struct miscdevice m = { 
		.minor = MISC_DYNAMIC_MINOR, 
		.name  			= "Vulnerable UAF Driver",
		.fops  			= NULL, 
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

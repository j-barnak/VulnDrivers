#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/phantom.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jared B");
MODULE_DESCRIPTION("Vulnerable UAF Driver :)");

// static ssize_t uaf_read(struct file* file, char __user *, 

static void set_file_operations(void) {
}

static int __init uaf_init(void) 
{
	set_file_operations();

	return 0;
}

static void __exit uaf_exit(void) 
{
	pr_debug("goodbye, exit\n");
}

module_init(uaf_init);
module_exit(uaf_exit);

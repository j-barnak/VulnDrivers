#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jared B");
MODULE_DESCRIPTION("Vulnerable UAF Module");

int uaf_init(void) 
{
	printk("uaf init");
	return 0;
}

void uaf_exit(void) 
{
	printk("uaf exit");
}

module_init(uaf_init);
module_exit(uaf_exit);

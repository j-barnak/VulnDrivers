#include <linux/module.h>
#include <linux/init.h>

#include "misc.h"

MODULE_LICENSE("GPL");

static int __init uaf_init(void)
{
	foo();
	return 1;
}

static void __exit uaf_exit(void)
{}

module_init(uaf_init);
module_exit(uaf_exit);

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

#include "./uaf.h"

int
uaf_alloc_object(unsigned long __user args)
{ 
	uaf_object_t *obj = kmalloc(sizeof(uaf_object_t), GFP_KERNEL);
	if (!obj) { 
		printk("Out of memory\n");
		return ENOMEM;
	}

	obj->args = args;
	obj->cb = &uaf_callback;

	uaf_object_context  = obj;

	pr_debug("== Allocated uaf_object_t ==");

	return 0;
}

int
uaf_use_object()
{
	if (!(uaf_object_context->cb)) {
		pr_debug("Null");
		return -1;
	}

	printk("Calling fn_ptr: 0x%p\n", uaf_object_context->cb);
	uaf_object_context->cb();

	return 0;
}

void
uaf_free_object()
{ 
	printk("== Freed uaf_object_t ==");
	kfree(uaf_object_context);
}

void
uaf_callback(void)
{
	printk("callback entered\n"); 
}

MODULE_LICENSE("GPL");

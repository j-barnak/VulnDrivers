#include "uaf.h"

#include <linux/kernel.h>
#include <linux/slab.h>

struct UafObject* global_uaf_object = NULL;

int alloc_uaf_obj(long arg)
{
    // NOTE: IS0 C90 forbids mixing declarations after code, so we put
    // 		 declarations at the top
    struct UafObject* target;

    pr_alert("[INFO] alloc_uaf_obj() called");

    target = kmalloc(sizeof(struct UafObject), GFP_KERNEL);

    if (!target) {
        pr_err("Unable to allocate object");
        return -ENOMEM;
    }

    target->function_ptr = callback;
    target->arg = arg;
    memset(target->buffer_one, 0x41, sizeof(target->buffer_one));
    target->buffer_one[55] = '\0'; // for string functions

    global_uaf_object = target;

    pr_alert("[INFO] target's function_ptr: %p", target->function_ptr);
    pr_alert("[INFO] target's arg: %li", target->arg);
    pr_alert("[INFO] target's buffer_one: %s", target->buffer_one);
    pr_alert("[INFO] uaf object allocated");

    return 0;
}

void use_uaf_obj()
{
    if (!global_uaf_object->function_ptr) {
        pr_alert("[INFO] global_uaf_object->function_ptr is NULL");
    }

    pr_alert("[INFO] use_uaf_obj() called");
    global_uaf_object->function_ptr();
    pr_alert("[INFO] global_uaf_object's function_ptr: %p",
        global_uaf_object->function_ptr);
}

// TODO: This is what I need to allocate
void alloc_k_obj(struct KernelObject* object)
{
    pr_alert("[INFO] alloc_k_obj(struct KernelObject* object) called");
}

void free_uaf_obj()
{
    pr_alert("[INFO] free_uaf_ob() called");
    kfree(global_uaf_object);
    pr_alert("[INFO] global_uaf_object freed");
}

void callback() { pr_alert("[INFO] callback invoked"); }

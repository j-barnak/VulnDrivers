#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#include "uaf.h"

MODULE_LICENSE("GPL");

struct miscdevice init_miscdevice(void)
{
    struct miscdevice m = {
        .minor = MISC_DYNAMIC_MINOR, .mode = 0777, .name = "V_Dev"};

    return m;
}

static int __init uaf_init(void)
{
    struct miscdevice m = init_miscdevice();

    return 1;
}

static void __exit uaf_exit(void) {}

module_init(uaf_init);
module_exit(uaf_exit);

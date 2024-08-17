#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GEORGE");
MODULE_DESCRIPTION("A simple hello kernelcare module");
MODULE_VERSION("0.1");

static int __init hello_kernelcare_init(void) {
    pr_debug("Hello, KernelCare!!");
    return 0;
}

static void __exit hello_kernelcare_exit(void) {
    pr_debug("Goodbye, KernelCare!");
}

module_init(hello_kernelcare_init);
module_exit(hello_kernelcare_exit);
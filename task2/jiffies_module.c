#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>

MODULE_AUTHOR("George");
MODULE_DESCRIPTION("Module jiffies");
MODULE_LICENSE("GPL");

static struct dentry *dir, *jiffies_file;

static ssize_t jiffies_read(struct file *file,
                            char __user *buf,
                            size_t len,
                            loff_t *ppos)
{
    char buffer[32];
    int written = snprintf(buffer, sizeof(buffer), "%lu\n", jiffies);
 
    return simple_read_from_buffer(buf, len, ppos, buffer, written);
}

static const struct file_operations jiffies_fops = 
{
    .read = jiffies_read,
};

static int __init hello_kernelcare_init(void)
{
    dir = debugfs_create_dir("kernelcare", NULL);
    if(!dir) {
        debugfs_remove_recursive(dir);
        return -1;
    }

    pr_debug("Hello, KernelCare!");
    return 0;
}

static void __exit hello_kernelcare_exit(void)
{
    pr_debug("Goodbye, KernelCare!");
    debugfs_remove_recursive(dir);
}

module_init(hello_kernelcare_init);
module_exit(hello_kernelcare_exit);

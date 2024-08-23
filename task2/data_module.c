// SPDX-License-Identifier: GPL-2.0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>

MODULE_AUTHOR("George");
MODULE_DESCRIPTION("A simple hello kernelcare module");
MODULE_LICENSE("GPL");

static struct dentry *dir, *jiffies_file, *data_file;
static char data_buffer[PAGE_SIZE];
static size_t data_size;
static DEFINE_MUTEX(data_lock);

static ssize_t jiffies_read(struct file *file,
							char __user *buf,
							size_t len,
							loff_t *ppos)
{
	char buffer[32];
	int written = snprintf(buffer, sizeof(buffer), "%lu\n", jiffies);

	return simple_read_from_buffer(buf, len, ppos, buffer, written);
}

static const struct file_operations jiffies_fops = {
	.read = jiffies_read,
};

static ssize_t data_read(struct file *file,
						char __user *buf,
						size_t len,
						loff_t *ppos)
{
	ssize_t ret;

	mutex_lock(&data_lock);
	ret = simple_read_from_buffer(buf, len, ppos, data_buffer, data_size);
	mutex_unlock(&data_lock);
	return ret;
}

static ssize_t data_write(struct file *file,
							const char __user *buf,
							size_t len,
							loff_t *ppos)
{
	ssize_t ret;

	mutex_lock(&data_lock);
	if (len > PAGE_SIZE)
		len = PAGE_SIZE;
	ret = simple_write_to_buffer(data_buffer, PAGE_SIZE, ppos, buf, len);
	if (ret >= 0)
		data_size = ret;
	mutex_unlock(&data_lock);
	return ret;
}

static const struct file_operations data_fops = {
	.read = data_read,
	.write = data_write,
};

static int __init hello_kernelcare_init(void)
{
	dir = debugfs_create_dir("kernelcare", NULL);
	if (!dir) {
		debugfs_remove_recursive(dir);
		return -1;
	}

	jiffies_file = debugfs_create_file("jiffies",
										0444,
										dir,
										NULL,
										&jiffies_fops);
	if (!jiffies_file) {
		debugfs_remove_recursive(dir);
		return -1;
	}

	data_file = debugfs_create_file("data",
									0444,
									dir,
									NULL,
									&data_fops);
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

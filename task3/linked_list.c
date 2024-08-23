// SPDX-License-Identifier: GPL-2.0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GEORGE");
MODULE_DESCRIPTION("A simple hello kernelcare module");


struct identity {
	char name[20];
	int id;
	bool hired;
	struct list_head list;
};

static LIST_HEAD(identity_pt);

/**
 * identity_create - creates the structure "identity",
 * copies in the ’name’ and ’id’fields and sets ’hired’ to false.
 * @name: the name of the list
 * @id: the id of the list
 */
int identity_create(char *name, int id)
{
	struct identity *new_identity;

	new_identity = kmalloc(sizeof(*new_identity), GFP_KERNEL);
	if (!new_identity)
		return -ENOMEM;

	strscpy(new_identity->name, name, sizeof(new_identity->name));
	new_identity->name[sizeof(new_identity->name) - 1] = '\0';
	new_identity->id = id;
	new_identity->hired = false;

	INIT_LIST_HEAD(&new_identity->list);
	list_add(&new_identity->list, &identity_pt);

	return 0;
}

/**
 * identity_find - iterates over the list of all ids,
 * and returns the proper ’struct identity’ associated with it.
 * @id: the id of the list
 */
struct identity *identity_find(int id)
{
	struct identity *id_pt;

	list_for_each_entry(id_pt, &identity_pt, list) {
		if (id_pt->id == id)
			return id_pt;
	}

	return NULL;
}

/**
 * identity_destroy - finds the proper ’struct identity’
 * and removes it from the system.
 * @id: the id of the list
 */
void identity_destroy(int id)
{
	struct identity *id_pt = identity_find(id);

	if (id_pt) {
		list_del(&id_pt->list);
		kfree(id_pt);
	}
}

/**
 * identity_hire - finds the proper ’struct identity’
 * and sets the ’hired’ field to true.
 * @id: the id of the list
 */
int identity_hire(int id)
{
	struct identity *id_pt = identity_find(id);

	if (!id_pt)
		return -ENOENT;

	id_pt->hired = true;
	return 0;
}

static int __init hello_kernelcare_init(void)
{
	struct identity *temp;
	int ret;

	pr_debug("Hello, KernelCare!");
	ret = identity_create("GEORGE", 1);
	if (ret) {
		pr_debug("Failed to create identity 1");
		// return -1;
	}

	ret = identity_create("MAKSAT", 2);
	if (ret) {
		pr_debug("Failed to create identity 2");
		// return -1;
	}

	temp = identity_find(1);
	if (temp == NULL)
		pr_debug("Failed to hire identity 1");
	else
		pr_debug("id 1 = %s\n", temp->name);

	ret = identity_hire(1);
	if (ret) {
		pr_debug("Failed to hire identity 1");
		// return -1;
	}

	temp = identity_find(10);
	if (temp == NULL)
		pr_debug("id 10 not found");
	identity_destroy(2);
	identity_destroy(1);

	return 0;
}

static void __exit hello_kernelcare_exit(void)
{
	struct list_head *entry, *tmp;
	struct identity *id_pt;

	pr_debug("Goodbye, KernelCare!");

	list_for_each_safe(entry, tmp, &identity_pt) {
		id_pt = (struct identity *)(entry->next);
		list_del(entry);
		kfree(id_pt);
		kfree(entry);
	}
}

module_init(hello_kernelcare_init);
module_exit(hello_kernelcare_exit);

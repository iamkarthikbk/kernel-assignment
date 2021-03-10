// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/string.h>

#define NAME_LENGTH 10

MODULE_LICENSE("GPL");
MODULE_AUTHOR("B K Karthik");
MODULE_DESCRIPTION("Linked Lists (Even after Bjarne Stroustrup asked us to avoid them)");

struct identity {
	char name[NAME_LENGTH];
	int id;
	bool busy;
	struct list_head list;
};

/* static variable that points to a list of these "identity" structures */
static LIST_HEAD(identity_list);

/* takes a given id, iterates over the list of all ids, and
 * returns the proper 'struct identity' associated with it.  If the
 * identity is not found, return NULL.
 */
struct identity *identity_find(int id)
{
	struct identity *temp_identity;

	list_for_each_entry(temp_identity, &identity_list, list) {
		if (temp_identity->id == id)
			return temp_identity;
	}

	return NULL;
}

/* creates the structure "identity", copies in the 'name' and 'id'
 * fields and sets 'busy' to false.  Proper error checking for out of
 * memory issues is required.  Return 0 if everything went ok; an error
 * value if something went wrong.
 */
int identity_create(char *name, int id)
{
	struct identity *temp_identity;
	int retval = -EINVAL;

	if (identity_find(id))
		return retval;

	temp_identity = kmalloc(sizeof(*temp_identity), GFP_KERNEL);
	if (!temp_identity)
		return retval;

	strncpy(temp_identity->name, name, NAME_LENGTH);
	temp_identity->name[NAME_LENGTH-1] = '\0';	/* EOL */
	temp_identity->id = id;
	temp_identity->busy = false;
	list_add(&(temp_identity->list), &identity_list);
	retval = 0;

	pr_debug("pesuio: identity %d: %s created successfully\n", id, name);

	return retval;
}

/* given an id, finds the proper 'struct identity' and removes it
 * from the system.
 */
void identity_destroy(int id)
{
	struct identity *temp_identity;

	temp_identity = identity_find(id);
	if (!temp_identity)
		return;

	pr_debug("pesuio: destroying identity %i: %s\n", temp_identity->id, temp_identity->name);
	list_del(&(temp_identity->list));
	kfree(temp_identity);
}

static int __init my_init(void)
{
	struct identity *temp_identity;

	pr_debug("Hello world!\n");
	if (identity_create("Alice", 1))
		pr_debug("Error creating identity \"Alice\"\n");
	if (identity_create("Bob", 2))
		pr_debug("Error creating identity \"Bob\"\n");
	if (identity_create("Dave", 3))
		pr_debug("Error creating identity \"Dave\"\n");
	if (identity_create("Gena", 10))
		pr_debug("Error creating identity \"Gena\"\n");

	temp_identity = identity_find(3);
	if (temp_identity == NULL)
		pr_debug("id 3 not found\n");
	else
		pr_debug("id 3 = %s\n", temp_identity->name);

	temp_identity = identity_find(42);
	if (temp_identity == NULL)
		pr_debug("id 42 not found\n");
	else
		pr_debug("id 42 = %s\n", temp_identity->name);

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
}

static void __exit my_exit(void)
{
	pr_debug("Exiting");
}

module_init(my_init);
module_exit(my_exit);

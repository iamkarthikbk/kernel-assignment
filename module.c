#include <linux/module.h> //required for all kernel modules
#include <linux/kernel.h> //for KERN_INFO
#include <linux/init.h>   //for __init and __exit macros

MODULE_LICENSE("GNU General Public License v2");
MODULE_AUTHOR("NAME");
MODULE_DESCRIPTION("Hello World Kernel Module");

static int __init my_init(void)
{
	printk(KERN_INFO "Hello world! I am NAME\n");
	return 0;
}

static void __exit my_exit(void)
{
	printk(KERN_INFO "Goodbye world!\n");
}

module_init(my_init);
module_exit(my_exit);
//The following webpage was used as a reference. https://www.thegeekstuff.com/2013/07/write-linux-kernel-module/

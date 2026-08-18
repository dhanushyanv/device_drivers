#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhanu");
MODULE_DESCRIPTION("A simple Hello World module");
static int __init hello_init()
{
  printk(KERN_INFO "Hello world");
  return 0;
}

static void __exit hello_exit()
{
  printk(KERN_INFO "Bye");
}

module_init(hello_init);
module_exit(hello_exit);

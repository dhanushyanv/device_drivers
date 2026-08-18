#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h
#include <linux/fs.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhanu");
MODULE_DESCRIPTION("A simple Character Device Driver");

static dev_t dev_num;
static struct cdev my_cdev;

#define BUFFER_SIZE 1024
static char kernel_buffer[BUFFER_SIZE];

static int char_driver_open(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "Character device opened\n");
  return 0;
}

static int char_driver_release(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "Character device closed\n");
  return 0;
}

static ssize_t char_driver_write(struct file* file, const char __user *user_buffer, size_t count,loff_t *offset)
{
	size_t bytes_to_copy;
	bytes_to_copy = min(count,(size_t)(BUFFER_SIZE -1));
	if(copy_from_user(kernel_buffer,user_buffer,bytes_to_copy)
	{
		printk(KERN_ERR "Failed to copy data from user");
			return -EFAULT:
	}

	kernel_buffer[bytes_to_copy] = '\0';
	printk(KERN_INFO "Received from user: %s \n",kernel_buffer);
	retun bytes_to_copy;
}

static const struct file_operation fops = {
  .owner = THIS_MODULE;
  .open = char_driver_open;
  .release = char_driver_release;
	.write = char_driver_write;

};

static int __init char_driver_init(void)
{
  int ret; 
  alloc_chrdev_region(dev_num,0,1,"my char device");
  if( ret <0)
  {
    printk(KERN_ALERT "Failed to allocate major number"\n);
    return ret;
  }
  printk("KERN_INFO "Major = %d , Minor=%d", MAJOR(dev_num), MINOR(dev_num));
  cdev_init(&my_cdev, &fops);
  ret = cdev_add(&my_cdev, dev_num,1);
  if(ret < 0)
  {
   unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "Failed to add cdev\n");
        return ret;
    }
    printk(KERN_INFO "Character Device Driver Initialized!\n");
    return 0;
}

static void __exit char_driver_exit(void)
{
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "Character Device Driver Exited!\n");
}
module_init(char_driver_init);
module_exit(char_driver_exit);


  



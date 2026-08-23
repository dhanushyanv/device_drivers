#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhanu");
MODULE_DESCRIPTION("A simple Character Device Driver");

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *char_class;
static size_t data_size = 0;

#define BUFFER_SIZE 1024
static char kernel_buffer[BUFFER_SIZE];

#define MY_CHAR_MAGIC 'D'
#define CLEAR_BUFFER _IO(MY_CHAR_MAGIC,1)
#define GET_DATA_SIZE _IOR(MY_CHAR_MAGIC,2,size_t)
#define RESET_DATA_SIZE _IO(MY_CHAR_MAGIC,3)

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
//Write function to handle data written from user space
static ssize_t char_driver_write(struct file* file, const char __user *user_buffer, size_t count,loff_t *offset)
{
	size_t bytes_to_copy;
	bytes_to_copy = min(count,(size_t)(BUFFER_SIZE));
	if(copy_from_user(kernel_buffer,user_buffer,bytes_to_copy))
	{
		printk(KERN_ERR "Failed to copy data from user");
			return -EFAULT;
	}
    data_size = bytes_to_copy;
	printk(KERN_INFO "Received from user: %s bytes=%zu\n",kernel_buffer, bytes_to_copy);
	return bytes_to_copy;
}

//Read function to handle data read from user space
static ssize_t char_driver_read(struct file* file, char __user *user_buffer, size_t count, loff_t *offset)
{
	size_t bytes_to_copy;
    if(*offset >= data_size)
    {
        return 0; // No more data to read
    }
    
	bytes_to_copy = min(count, data_size - (size_t)(*offset));
	if(copy_to_user(user_buffer,kernel_buffer + *offset,bytes_to_copy))
	{
		printk(KERN_ERR "Failed to copy data to user");
			return -EFAULT;
	}
    *offset += bytes_to_copy;

	printk(KERN_INFO "Sent to user: %s \n",kernel_buffer);
	return bytes_to_copy;
}
// IO CTL commands
static long my_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
{
  switch(cmd)
    {
      case CLEAR_BUFFER:
        {
          memset(kernel_buffer,0,BUFFER_SIZE);
          data_size =0;
          printk(KERN_INFO "Ioctl: buffer cleared");
          break;
        }
      case GET_DATA_SIZE:
        {
          if(copy_to_user((size_t __user *)arg, &data_size, sizeof(size_t)))
          {
            printk(KERN_ERR "Failed to copy data size to user");
            return -EFAULT;
          }
          break;
        }
      case RESET_DATA_SIZE:
        {
          data_size = 0;
          printk(KERN_INFO "Ioctl: data size reset");
          break;
        }
      default:
        return -EINVAL;
    }
  return 0;
}
// Define file operations structure
static const struct file_operations fops = {
  .owner = THIS_MODULE,
  .open = char_driver_open,
  .release = char_driver_release,
  .write = char_driver_write,
  .read = char_driver_read,
  .unlocked_ioctl = my_ioctl,
};

static int __init char_driver_init(void)
{
    printk(KERN_INFO "char dev: Character Device Driver Initializing...\n");
  int ret; 
  //Allocate a major and minor number for the device
  ret = alloc_chrdev_region(&dev_num,0,1,"my char device");
  if( ret <0)
  {
    printk(KERN_ALERT "Failed to allocate major number\n");
    return ret;
  }
  printk(KERN_INFO "char dev: Major = %d , Minor=%d", MAJOR(dev_num), MINOR(dev_num));

  //Initialize the cdev structure and add it to the kernel
  cdev_init(&my_cdev, &fops);

  //Add the character device to the system
  ret = cdev_add(&my_cdev, dev_num,1);
  if(ret < 0)
  {
   unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "Failed to add cdev\n");
        return ret;
    }

    //Create a device class and device node in /dev
    char_class = class_create(THIS_MODULE, "my_char_class");
    if(IS_ERR(char_class))
    {
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "Failed to create class\n");
        return PTR_ERR(char_class);
    }

    //Create the device node in /dev
    if(IS_ERR(device_create(char_class, NULL, dev_num, NULL, "mychar")))
    {
        class_destroy(char_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "Failed to create device\n");
        return -1;
    }

    printk(KERN_INFO "char dev: Character Device Driver Initialized!\n");
    return 0;
}

static void __exit char_driver_exit(void)
{
    device_destroy(char_class, dev_num);
    class_destroy(char_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "char dev: Character Device Driver Exited!\n");
}
  
module_init(char_driver_init);
module_exit(char_driver_exit);

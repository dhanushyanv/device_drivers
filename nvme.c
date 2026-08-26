#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhanushya");
MODULE_DESCRIPTION("A Mini NVMe Device Driver");

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *char_class;

//NVME commands
#define NVME_WRITE 0x01
#define NVME_READ 0x02
#define NVME_DATA_SIZE 256

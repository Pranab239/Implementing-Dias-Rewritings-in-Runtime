#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <asm/io.h>
 
#include "query_ioctl.h"
 
#define FIRST_MINOR 0
#define MINOR_CNT 1
 
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
int result;
 
static int my_open(struct inode *i, struct file *f)
{
    return 0;
}

static int my_close(struct inode *i, struct file *f)
{
    return 0;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    query_arg_t q;
    long unsigned int *virtual_addr;
 
    switch (cmd)
    {
        case VIR_TO_PHYS_TRANS:
            if (copy_from_user(&q, (query_arg_t *)arg, sizeof(query_arg_t))) {
                pr_err("Failed to copy virtual address from user space\n");
                return -EFAULT;
            }

            // Translate virtual to physical address
            q.phys_addr = virt_to_phys((void *)q.v_addr);

            // Copy the result to user space
            if (copy_to_user((unsigned long *)arg, &q, sizeof(query_arg_t))) {
                pr_err("Failed to copy physical address to user space\n");
                return -EFAULT;
            }
            break;
        
        case PUT_VALUE_IN_ADDR:
            if (copy_from_user(&q, (query_arg_t *)arg, sizeof(query_arg_t))) {
                pr_err("Failed to copy virtual address from user space\n");
                return -EFAULT;
            }

            // Translate virtual to physical address
            pr_info("The user given value: %d and address %lx", q.value, q.phys_addr);
            virtual_addr = (unsigned long*)phys_to_virt(q.phys_addr);
            pr_info("Before: The value at virtual address %lx is %ld", (unsigned long)q.v_addr, *(virtual_addr));

            // Assign the value at specific address
            *(virtual_addr) = q.value;
            
            // Copy the result to user space
            if (copy_to_user((unsigned long *)arg, &q, sizeof(query_arg_t))) {
                pr_err("Failed to put value to physical address to user space\n");
                return -EFAULT;
            }
            break;
        default:
            return -EFAULT;
    }
 
    return 0;
}
 
static struct file_operations query_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = my_ioctl
#else
    .unlocked_ioctl = my_ioctl
#endif
};
 
static int __init query_ioctl_init(void)
{
    int ret;
    struct device *dev_ret;
 
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "query_ioctl")) < 0)
    {
        return ret;
    }
 
    cdev_init(&c_dev, &query_fops);
 
    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }
     
    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "query")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }
 
    return 0;
}
 
static void __exit query_ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
}
 
module_init(query_ioctl_init);
module_exit(query_ioctl_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pranab Paul");
MODULE_DESCRIPTION("Address Translation and write value at memory address.");
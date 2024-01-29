#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <asm/io.h>
 
#include "help_dr_doom.h"
 
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
    int parent_pid;
    pid_t current_pid;
    struct task_struct *current_task, *parent_task;
 
    switch (cmd)
    {
        case CHANGE_PARENT:
            if (copy_from_user(&parent_pid, (int *)arg, sizeof(int))) {
                pr_err("Failed to copy parent pid from user space\n");
                return -EFAULT;
            }

            // Get the current pid of the caller porgram
            printk(KERN_INFO "The user given parent pid is: %d\n", parent_pid);
            current_pid = current->pid;
            printk(KERN_INFO "The pid of the caller is: %d\n", current_pid);

            // Accessing the current process task 
            current_task = current;
            parent_task = pid_task(find_get_pid(parent_pid), PIDTYPE_PID);

            // put the parent into the child task
            current_task->parent = parent_task;
            current_task->real_parent = parent_task;

            // put the child into the parent task
            list_add_tail(&current_task->sibling, &parent_task->children);

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
MODULE_AUTHOR("Pranab");
MODULE_DESCRIPTION("Help dr.doom to manage his loyad soldiers.");
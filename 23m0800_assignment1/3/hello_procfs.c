#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define PROCFS_ENTRY "hello_procfs"

static int show_hello_procfs(struct seq_file *m, void *v) {
    seq_printf(m, "Hello World!\n");
    return 0;
}

static int hello_procfs_open(struct inode *inode, struct file *file) {
    return single_open(file, show_hello_procfs, NULL);
}

static const struct proc_ops hello_procfs_fops = {
    .proc_open = hello_procfs_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init hello_procfs_init(void) {
    proc_create(PROCFS_ENTRY, 0, NULL, &hello_procfs_fops);
    printk(KERN_INFO "Hello Procfs Module Initialized\n");
    return 0;
}

static void __exit hello_procfs_exit(void) {
    remove_proc_entry(PROCFS_ENTRY, NULL);
    printk(KERN_INFO "Hello Procfs Module Removed\n");
}

module_init(hello_procfs_init);
module_exit(hello_procfs_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pranab");
MODULE_DESCRIPTION("Hello_Procfs Module");

#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/vmstat.h>

#define PROCFS_ENTRY "get_pgfaults"

static int show_page_faults(struct seq_file *m, void *v) {
    unsigned long pgfaults[NR_VM_EVENT_ITEMS];
    all_vm_events(pgfaults);

    // Display page fault information
    seq_printf(m, "Page Faults: %lu\n", pgfaults[PGFAULT]);

    return 0;
}

static int open_get_pgfaults(struct inode *inode, struct file *file) {
    return single_open(file, show_page_faults, NULL);
}

static const struct proc_ops get_pgfaults_fops = {
    .proc_open = open_get_pgfaults,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init get_pgfaults_init(void) {
    proc_create(PROCFS_ENTRY, 0, NULL, &get_pgfaults_fops);
    printk(KERN_INFO "get_pgfaults Module Initialized\n");
    return 0;
}

static void __exit get_pgfaults_exit(void) {
    remove_proc_entry(PROCFS_ENTRY, NULL);
    printk(KERN_INFO "get_pgfaults Module Removed\n");
}

module_init(get_pgfaults_init);
module_exit(get_pgfaults_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pranab");
MODULE_DESCRIPTION("Page Faults Module");

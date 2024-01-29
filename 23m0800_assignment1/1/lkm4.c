#include <linux/sched.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/io.h>
#include <linux/mm_types.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>

// Default parent PID is 1 (init process)
static int pid = 1;
unsigned long total_physical_size;
unsigned long size = 0;
struct page *page;
int count = 0;
int level;

/* Taking input PID */
module_param(pid, int, S_IRUGO);

static int __init lkm_init(void) {
    
    struct task_struct *task;
    struct vm_area_struct *vma;
    struct mm_struct *mm;

    pr_info("Virtual Address Space Calculation Module Loaded\n");

    // Find the task_struct for the given PID
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (!task) {
        pr_err("Process with PID %d not found\n", pid);
        return -1;
    }

    // Access the process's memory space
    mm = task->mm;
    if (!mm) {
        pr_err("Failed to get memory space for PID %d\n", task->pid);
        return 0;
    }

    // Iterate over the VMAs and calculate the total virtual address size
    mmap_read_lock(mm);

    VMA_ITERATOR(iter, mm, 0);
    for_each_vma(iter, vma) {
        size += vma->vm_end - vma->vm_start;
    }

    // The value obtained from get_mm_rss() is the number of pages, 
    // you will have to multiply this by the page size (simply shift left by PAGE_SHIFT).
    total_physical_size = get_mm_rss(mm) << PAGE_SHIFT;

    mmap_read_unlock(mm);

    pr_info("Total Virtual address for PID %d is %ld bytes.\n", pid, size);
    pr_info("Total Physical address for PID %d is %ld bytes.\n", pid, total_physical_size);
    return 0;
}

static void __exit lkm_exit(void) {
    pr_info("Virtual Address Space Calculation Module Unloaded\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

/* Meta Information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pranab Paul");
MODULE_DESCRIPTION("Address Space Calculation Kernel Module");
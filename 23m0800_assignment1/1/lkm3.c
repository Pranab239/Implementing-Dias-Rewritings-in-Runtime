#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/io.h>

/* Taking the pid from the input */
static int pid = 1;
module_param(pid, int, S_IRUGO);

/* Taking the virtual address from the input */
unsigned long vaddr = 0x12345678;
module_param(vaddr, long, S_IRUGO);

unsigned long page_no;
unsigned long physical_add;

static void print_address_translation(void) {
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    struct page *pages = NULL;

    /* Find the task_struct for the given PID */
    rcu_read_lock();
    task = pid_task(find_get_pid(pid), PIDTYPE_PID);
    rcu_read_unlock();

    /* Validating task_struct of the given PID */
    if (!task) {
        pr_err("Process with PID %d not found\n", pid);
        return;
    }

    pr_info("Process with PID %d found\n", pid);

    /* Access the process's memory space */
    mm = get_task_mm(task);
    if (!mm) {
        pr_err("Failed to get memory space for PID %d\n", pid);
        return;
    }

    down_read(&mm->mmap_lock);
    
    /* Get the virtual memory address space from the virtual address */
    vma = vma_lookup(mm, vaddr);
    /* Get the page for the corresponding virtual address */
    get_user_pages_remote(mm, vma->vm_start, mm->total_vm, FOLL_FORCE, &pages, NULL, NULL);
    /* Get the base address of the page */
    page_no = page_to_phys(pages);
    /* Get the physical address after adding the offset of the virtual address with base address */
    physical_add = page_no +  offset_in_page(vaddr);

    up_read(&mm->mmap_lock);

    pr_info("The physical address of corresponding virtual address %lx for pid %d is : %lx", vaddr, pid, physical_add);
}

static int __init lkm_init(void) {

    pr_info("Address Translation Kernel Module Loaded\n");
    print_address_translation();
    return 0;
}

static void __exit lkm_exit(void) {
    pr_info("Address Translation Kernel Module Unloaded\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pranab Paul");
MODULE_DESCRIPTION("Address Translation Kernel Module");

#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pranab Paul");
MODULE_DESCRIPTION("Assignment of listing all Running or Runnable Processes");

static int __init lkm_init(void) {
    struct task_struct *task;

    pr_info("Kernel Module loaded\n\n");
    pr_info("List of Running or Runnable Processes:\n");

    // Traverse the task list
    for_each_process(task) {

        // Check if the process is in a running or runnable state
        if (task->__state == TASK_RUNNING || task->__state == TASK_INTERRUPTIBLE) {
            pr_info("PID: %d\n", task->pid);
        }
    }
    return 0;
}

static void __exit lkm_exit(void) {
    pr_info("\nKernel module unloaded\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

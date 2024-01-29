#include <linux/sched.h>
#include <linux/init.h>
#include <linux/module.h>

/* Default parent PID is 1 (init process) */
static int parent_pid = 1;

/* Taking the parent pid from the input */
module_param(parent_pid, int, S_IRUGO);

static int __init traverse_children_init(void) {
    struct task_struct *parent_task, *child_task;
    
    pr_info("LKM2 Module loaded\n");
    pr_info("Usage :: sudo insmod lkm2.ko parent_pid=XXXX\n");
    rcu_read_lock();

    /* Find the parent process */
    parent_task = pid_task(find_get_pid(parent_pid), PIDTYPE_PID);
    if (!parent_task) {
        pr_err("Parent process with PID %d not found\n", parent_pid);
        rcu_read_unlock();
        return -EINVAL;
    }

    pr_info("List of child PIDs and their States:\n");

    /* Traverse and print information about child processes */
    list_for_each_entry_rcu(child_task, &parent_task->children, sibling) {
        pr_info("Child PID: %d of Parent PID: %d with STATE: %d\n", child_task->pid, parent_task->pid, child_task->__state);
    }

    rcu_read_unlock();
    return 0;
}

static void __exit traverse_children_exit(void) {
    pr_info("LKM2 Module unloaded\n");
}

module_init(traverse_children_init);
module_exit(traverse_children_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pranab Paul");
MODULE_DESCRIPTION("Traverse Children Module");

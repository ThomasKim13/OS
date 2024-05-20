#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/mutex.h>

struct job_t {
    pid_t pid;
    int jobTime;
    int remainingTime;
    int priority;
    struct list_head list;
};

static LIST_HEAD(waiting_queue);
static struct job_t *current_job = NULL;
static DEFINE_MUTEX(ku_cpu_mutex);

static struct job_t* get_next_rr_job(void) {
    if (!list_empty(&waiting_queue)) {
        struct job_t *job = list_first_entry(&waiting_queue, struct job_t, list);
        list_del(&job->list);
        return job;
    }
    return NULL;
}

static struct job_t* get_next_srtf_job(void) {
    struct job_t *shortest_job = NULL;
    struct job_t *job;

    if (!list_empty(&waiting_queue)) {
        list_for_each_entry(job, &waiting_queue, list) {
            if (shortest_job == NULL || job->remainingTime < shortest_job->remainingTime) {
                shortest_job = job;
            }
        }
        if (shortest_job != NULL) {
            list_del(&shortest_job->list);
        }
    }
    return shortest_job;
}

static struct job_t* get_next_priority_job(void) {
    struct job_t *highest_priority_job = NULL;
    struct job_t *job;

    if (!list_empty(&waiting_queue)) {
        list_for_each_entry(job, &waiting_queue, list) {
            if (highest_priority_job == NULL || job->priority > highest_priority_job->priority) {
                highest_priority_job = job;
            }
        }
        if (highest_priority_job != NULL) {
            list_del(&highest_priority_job->list);
        }
    }
    return highest_priority_job;
}

SYSCALL_DEFINE5(os2024_ku_cpu, char __user *, name, int, jobTime, int, policy, int, priority, int, time_quantum) {
    pid_t pid = current->pid;
    struct job_t *job;
    int ret = 1;

    mutex_lock(&ku_cpu_mutex);

    if (current_job == NULL) {
        current_job = kmalloc(sizeof(struct job_t), GFP_KERNEL);
        current_job->pid = pid;
        current_job->jobTime = jobTime;
        current_job->remainingTime = jobTime;
        current_job->priority = priority;
        printk(KERN_INFO "Process %s (PID: %d) starts using CPU.\n", name, pid);
        ret = 0;
    } else if (current_job->pid == pid) {
        current_job->remainingTime -= 1;
        if (current_job->remainingTime <= 0) {
            printk(KERN_INFO "Process %s (PID: %d) finished using CPU.\n", name, pid);
            kfree(current_job);
            if (list_empty(&waiting_queue)) {
                current_job = NULL;
            } else {
                if (policy == 1) { // Round-Robin
                    current_job = get_next_rr_job();
                } else if (policy == 2) { // SRTF
                    current_job = get_next_srtf_job();
                } else if (policy == 3) { // Priority
                    current_job = get_next_priority_job();
                }
                if (current_job != NULL) {
                    printk(KERN_INFO "Process (PID: %d) starts using CPU from queue.\n", current_job->pid);
                }
            }
        }
        ret = 0;
    } else {
        job = kmalloc(sizeof(struct job_t), GFP_KERNEL);
        job->pid = pid;
        job->jobTime = jobTime;
        job->remainingTime = jobTime;
        job->priority = priority;
        if (policy == 2) { // SRTF
            list_for_each_entry_reverse(current_job, &waiting_queue, list) {
                if (current_job->remainingTime > jobTime) {
                    list_add(&job->list, current_job->list.prev);
                    break;
                }
            }
            if (&current_job->list == &waiting_queue) {
                list_add_tail(&job->list, &waiting_queue);
            }
        } else if (policy == 3) { // Priority
            list_for_each_entry_reverse(current_job, &waiting_queue, list) {
                if (current_job->priority < priority) {
                    list_add(&job->list, current_job->list.prev);
                    break;
                }
            }
            if (&current_job->list == &waiting_queue) {
                list_add_tail(&job->list, &waiting_queue);
            }
        } else {
            list_add_tail(&job->list, &waiting_queue);
        }
        printk(KERN_INFO "Process %s (PID: %d) added to waiting queue.\n", name, pid);
        ret = 1;
    }

    mutex_unlock(&ku_cpu_mutex);
    return ret;
}


#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

#define QUEUE_SIZE 100
static int queue[QUEUE_SIZE];
static int front = -1;
static int rear = -1;

static int enqueue(int value) {
    if ((rear + 1) % QUEUE_SIZE == front) {
        printk(KERN_INFO "Enqueue failed: Queue full\n");
        return -1;
    } else {
        if (front == -1) {
            front = 0;
        }
        rear = (rear + 1) % QUEUE_SIZE;
        queue[rear] = value;
        printk(KERN_INFO "Enqueued value: %d\n", value);
	printk(KERN_INFO "QUEUE SYSCALL \n");
        return 0;
    }
}

static int dequeue(void) {
    int value;
    if (front == -1) {
        printk(KERN_INFO "Dequeue failed: Queue empty\n");
        return -1;
    } else {
        value = queue[front];
        if (front == rear) {
            front = rear = -1;
        } else {
            front = (front + 1) % QUEUE_SIZE;
        }
        printk(KERN_INFO "Dequeued value: %d\n", value);
	printk(KERN_INFO "DEQUEUE SYSCALL \n");
        return value;
    }
}

SYSCALL_DEFINE1(os2024_enqueue, int, value) {
    return enqueue(value);
}

SYSCALL_DEFINE0(os2024_dequeue) {
    return dequeue();
}


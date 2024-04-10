#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

#define QUEUE_SIZE 100
static int queue[QUEUE_SIZE];
static int front = -1;
static int rear = -1;
//Simple enqueue function that is learned from Data Structures course
static int enqueue(int value) {
    if ((rear + 1) % QUEUE_SIZE == front) {
        printk(KERN_INFO "Enqueue failed: Queue full\n");
        return -1;
	    //the enqueue algorithim
    } else {
        if (front == -1) {
            front = 0;
        }
        rear = (rear + 1) % QUEUE_SIZE;
        queue[rear] = value;
	    //Added print function for dmesg to show that the Syscall works
        printk(KERN_INFO "Enqueued value: %d\n", value);
	printk(KERN_INFO "QUEUE SYSCALL \n");
        return 0;
    }
}
//Simple dequeue function that is learned from Data Structures Course
static int dequeue(void) {
    int value;
	//if there is nothing in the queue then the error is handled to avoid SegFaults
    if (front == -1) {
        printk(KERN_INFO "Dequeue failed: Queue empty\n");
        return -1;
	    //The dequeue algorithim
    } else {
        value = queue[front];
        if (front == rear) {
            front = rear = -1;
        } else {
            front = (front + 1) % QUEUE_SIZE;
        }
	    //Added print function for dmesg to show that the Syscall works
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


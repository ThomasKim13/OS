/* 2024 Spring COSE341 Operating System */
/* Project 1 */
/* KIM CHULJUN */


#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

#define QUEUE_SIZE 100
static int queue[QUEUE_SIZE];
static int front = -1;
static int rear = -1;

// Simple enqueue function
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
        return 0;
    }
}

// Simple dequeue function
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
        return value;
    }
}

SYSCALL_DEFINE1(os2024_enqueue, int, value) {
    int result = enqueue(value);
    if (result == 0) {
        int i;
        printk(KERN_INFO "os2024_enqueue syscall was called\n");
        printk(KERN_INFO "queue front---------\n");
        if (front != -1) {
            for (i = front; i != (rear + 1) % QUEUE_SIZE; i = (i + 1) % QUEUE_SIZE)
                printk(KERN_INFO "%d\n", queue[i]);
        }
        printk(KERN_INFO "queue end---------\n");
    }
    return result;
}

SYSCALL_DEFINE0(os2024_dequeue) {
    int result = dequeue();
    if (result != -1) {
        int i;
        printk(KERN_INFO "os2024_dequeue syscall was called\n");
        printk(KERN_INFO "queue front---------\n");
        if (front != -1) {
            for (i = front; i != (rear + 1) % QUEUE_SIZE; i = (i + 1) % QUEUE_SIZE)
                printk(KERN_INFO "%d\n", queue[i]);
        }
        printk(KERN_INFO "queue end---------\n");
    }
    return result;
}


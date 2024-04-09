#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#define MY_QUEUE_ENQUEUE 335
#define MY_QUEUE_DEQUEUE 336

// Function to generate a random number between min and max (inclusive)
int generate_random_number(int min, int max) {
    return rand() % (max - min + 1) + min;
}

int main() {
    // Seed the random number generator
    srand(time(NULL));

    // Enqueue three random values
    for (int i = 0; i < 3; ++i) {
        int value = generate_random_number(1, 100); // Generating a random number between 1 and 100
        if (syscall(MY_QUEUE_ENQUEUE, value) == -1) {
            perror("Error enqueuing value");
            return 1;
        }
        printf("Enqueued value: %d\n", value);
    }

    // Dequeue three values
    for (int i = 0; i < 3; ++i) {
        int value = syscall(MY_QUEUE_DEQUEUE);
        if (value == -1) {
            perror("Error dequeuing value");
            return 1;
        }
        printf("Dequeued value: %d\n", value);
    }
    return 0;
}


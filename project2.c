#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/syscall.h>

#define KU_CPU 339 // define syscall number

int main(int argc, char **argv) {
    int jobTime;
    int delayTime;
    int policy;
    int priority;
    int time_quantum;
    char name[14];
    int wait = 0;
    struct timeval start, end, first_response;
    int first_response_time_set = 0;

    if (argc < 6) {
        printf("Insufficient Arguments..\n");
        return 1;
    }

    /* first argument: job time (second)
       second argument: delay time before execution (second)
       third argument: process name
       fourth argument: scheduling policy (0: FCFS, 1: RR, 2: SRTF, 3: Priority)
       fifth argument: priority (only used for Priority scheduling)
       sixth argument: time quantum (only used for RR scheduling) */

    jobTime = atoi(argv[1]);
    delayTime = atoi(argv[2]);
    strcpy(name, argv[3]);
    policy = atoi(argv[4]);
    priority = atoi(argv[5]);
    time_quantum = atoi(argv[6]);

    // wait for 'delayTime' seconds before execution
    sleep(delayTime);
    gettimeofday(&start, NULL);
    printf("Process %s: I will use CPU for %ds.\n", name, jobTime);
    jobTime *= 10; // execute system call in every 0.1 second

    // Continue requesting the system call as long as the jobTime remains
    while (jobTime) {
        // If request is rejected, increase wait time
        if (syscall(KU_CPU, name, jobTime, policy, priority, time_quantum) == 0) {
            if (!first_response_time_set) {
                gettimeofday(&first_response, NULL);
                first_response_time_set = 1;
            }
            jobTime--;
        } else {
            wait++;
        }
        usleep(100000); // delay 0.1 second
    }

    gettimeofday(&end, NULL);
    int response_time = (first_response.tv_sec - start.tv_sec) * 1000 + (first_response.tv_usec - start.tv_usec) / 1000;
    int total_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
    printf("Process %s: Finish! My total wait time is %dms. My response time is %dms.\n", name, (wait * 100), response_time);
    return 0;
}


// main.c

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "scheduler.h"

#define MAX_WORKLOAD_SIZE 100
#define CPU_size 20

int main() {
    // source file
    const char *filename = "sched.txt";
    int amountProcess;
    // array to store CPU processes
    workload_item workload[MAX_WORKLOAD_SIZE];

    // load processes from text file to the workload array
    read_sched_file(filename, workload, &amountProcess);

    schedule(workload, amountProcess, CPU_size);

    // print final output that shows workflow of processes
    printFinalResult(workload, amountProcess);

    return 0;
}

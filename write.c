#include "scheduler.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>


//Write Queue in txt file
void writeQueue(FILE *file, char *q_name, queue_item *q, int q_count){
    fprintf(file, "%s: [", q_name);
    for (int i = 0; i < q_count; i++)
    {
        fprintf(file, "(%d, %d)", q[i].prio, q[i].pid);
    }
    fprintf(file, "]\n");
}

// Write a message on terminal about process that finished
void writeFInishMessage(FILE *file, queue_item *running_queue, int running_process_index, workload_item *workload){
    int pid = running_queue[running_process_index].pid;
    int prio = running_queue[running_process_index].prio;
    char* binaryName = workload[pid].cmd;

    fprintf(file, "process pid=%d prio=%d ('%s') finished after time t=%ld\n", pid, prio, binaryName, workload[pid].tf);
}
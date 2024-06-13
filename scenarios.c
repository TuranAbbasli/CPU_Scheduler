#include "scheduler.h"
#include "write.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

// Scenario when a process is needed to finish.
void finishScenario(FILE *file, queue_item *running_queue, int *running_count, 
                    int *running_process_index, workload_item *workload, int *cpu_occupation)
{
    // write finish message in txt file
    writeFInishMessage(file, running_queue, *running_process_index, workload);
    // decrease cpu occupation
    *cpu_occupation -= running_queue[*running_process_index].prio;
    fprintf(file, "CPU occupation: %d\n", *cpu_occupation);
    
    // finish process
    finishProcess(running_queue, *running_process_index, running_count);

    // decreament index as process on index+1 is moved to index of removed process
    (*running_process_index)--;
}


// Scenario when a process is need to start running, and CPU has space.
void runScenario_CPU_has_space(FILE *file, queue_item *running_queue, int *running_count, queue_item *pending_queue, int *pending_count, 
                                int pending_process_index, int *cpu_occupation, workload_item *workload)
{
    queue_item pending_process = pending_queue[pending_process_index];

    // run process
    runProcess(running_queue, running_count, pending_process);
    fprintf(file, "schedule pid=%d prio=%d ('%s') ... added to running queue\n", pending_process.pid, pending_process.prio, workload[pending_process.pid].cmd);
    *cpu_occupation += pending_process.prio;
    fprintf(file, "CPU occupation: %d\n", *cpu_occupation);

    // remove process from pending queue
    removeProcessFromPendingQueue(pending_queue, pending_process_index, pending_count);
}


// Scenario when a process is need to run, but CPU has not enough space. As it is Higher priority case some running process will be setted to sleep.
void handle_CPU_has_no_space_Higher_Priority(FILE *file, queue_item *running_queue, int *running_count, queue_item *pending_queue, int *pending_count,
                                                moveProcess check, int pending_process_index, int *cpu_occupation, workload_item *workload)
{   
    queue_item pending_process = pending_queue[pending_process_index];

    sleepRunningProcesses(file, running_queue, running_count, pending_queue, pending_count, check.index, workload, cpu_occupation);
    // run process after making space in cpu
    fprintf(file, "schedule pid=%d prio=%d ('%s') ... added to running queue\n", pending_process.pid, pending_process.prio, workload[pending_process.pid].cmd);
    runProcess(running_queue, running_count, pending_process);

    // remove process from pending queue
    removeProcessFromPendingQueue(pending_queue, pending_process_index, pending_count);

    *cpu_occupation += pending_process.prio;
    fprintf(file, "CPU occupation: %d\n", *cpu_occupation);
}


// Scenario when a process is need to run, but CPU has not enough space. As it is Lower priority case, process will not be moved and will be setted to idle-sleep.
void handle_CPU_has_no_space_Lower_Priority(FILE *file, queue_item *running_queue, moveProcess check, 
                                                int cpu_occupation, queue_item pending_process, workload_item *workload)
{
    fprintf(file, "None, as min prio: pid=%d prio=%d ", running_queue[check.index].pid, running_queue[check.index].prio);
    fprintf(file, "('%s') has graeter priority\n", workload[running_queue[check.index].pid].cmd);
    fprintf(file, "CPU occupation: %d\n", cpu_occupation);

    // as pending process stays in pending queue, increment both idle and tf values of that process in workload array
    workload[pending_process.pid].idle++;
    workload[pending_process.pid].tf++;
}

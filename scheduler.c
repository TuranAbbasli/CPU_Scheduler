// scheduler.c

#include "scheduler.h"
#include "initialize.h"
#include "write.h"
#include "scenarios.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 20

// read source data of processes from sched.txt file
void read_sched_file(const char *filename, workload_item *workload, int *amountProcess) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(1);
    }

    int pid, ppid, prio;
    size_t ts, tf, idle;
    char cmd[100];

    int count = 0;
    while (fscanf(file, "%d %d %zu %zu %zu %s %d\n", &pid, &ppid, &ts, &tf, &idle, cmd, &prio) == 7) {
        workload[count].pid = pid;
        workload[count].ppid = ppid;
        workload[count].ts = ts;
        workload[count].tf = tf;
        workload[count].idle = idle;
        workload[count].cmd = malloc(strlen(cmd) + 1);
        if (workload[count].cmd == NULL) {
            fprintf(stderr, "Error allocating memory\n");
            exit(1);
        }
        strcpy(workload[count].cmd, cmd);
        workload[count].prio = prio;
        count++;
    }

    *amountProcess = count;

    fclose(file);
}


// check if it is time to finish a running process
bool checkFinish(queue_item running_process, workload_item *workload, int timestep){
    if (workload[running_process.pid].tf+1 == timestep)
    {
        return true;
    }
    
    return false;
}


// finish a process running
void finishProcess(queue_item *running_queue, int running_process_index, int *running_count){
    for (int i = running_process_index; i < (*running_count)-1; i++)
    {
        running_queue[i] = running_queue[i+1];
    }

    running_queue[(*running_count)-1].pid = -1;
    running_queue[(*running_count)-1].prio = -1;
    
    // Decrement the count of running processes
    (*running_count)--;

}


// uses binary search to get index where the process will be placed in new queue where it is moving
int binarySearch(queue_item *q, queue_item movingProcess, int process_count){
    
    int left = 0;
    int right = process_count - 1;

    // if runnig queue is empty return index 0
    if (process_count == 0)
    {
        return left;
    }
    // binary search
    else{
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (q[mid].prio == movingProcess.prio){
                return mid;
            }
            if (q[mid].prio < movingProcess.prio){
                right = mid - 1;
            }
            else{
                left = mid + 1;
            }
        }
        return left;
    }


}



// Insert a process into a queue
void pushProcess(queue_item *q, queue_item process, int process_count){
    // search of index of new process in destination queue
    int index = binarySearch(q, process, process_count);

    if (index == 0)
    {
        q[index] = process;
    }
    else{
        for (int i = process_count-1; i >= index; i--)
        {
            q[i+1] = q[i];
        }
        
        // insert new process
        q[index] = process;
    }

}


// to run process move it to runnig queue
void runProcess(queue_item *running_queue, int *running_count, queue_item pending_process){  
    // push process into running queue
    pushProcess(running_queue, pending_process, *running_count);

    // increment running process count
    (*running_count)++;
    
}

// remove process, that runs, from pending queue
void removeProcessFromPendingQueue(queue_item *pending_queue, int pending_process_index, int *pending_count){
    for (int i = pending_process_index; i < (*pending_count)-1; i++)
    {
        pending_queue[i] = pending_queue[i+1];
    }

    pending_queue[(*pending_count)-1].pid = -1;
    pending_queue[(*pending_count)-1].prio = -1;

    // Decrement the count of pending processes
    (*pending_count)--;
    
}


// Function to check if process can be run in case cpu is occupied. 
// Returns structure containing possible index of process in running queue and bool value representing possiblity
moveProcess checkProcessMove(queue_item *running_queue, int running_count, int cpu_occupation, int CPU_size, queue_item pending_process){
    moveProcess result;
    int space;

    // iterate from lowest prio to highest prio process in running queue
    for (int i = running_count-1; i >= 0; i--)
    {   
        // if prio of process in running queue is >= it is not possible to run pending process
        if (running_queue[i].prio >= pending_process.prio)
        {
            result.index = i;
            result.possible = false;
            break;
        }
        
        cpu_occupation -= running_queue[i].prio;
        space = CPU_size - cpu_occupation;
        // if enough space has been opened for pending process in CPU, it is possible to run process
        if (space >= pending_process.prio)
        {
            result.index = i;
            result.possible = true;
            
            break;
        }
        
    }
    
    return result;

}

// sleep running process by moving it to pending queue.
void sleepRunningProcesses(FILE *file, queue_item *running_queue, int *running_count, queue_item *pending_queue, int *pending_count, int newIndex_of_PendingProcess, workload_item *workload, int *cpu_occupation){
    for (int i = newIndex_of_PendingProcess; i < *running_count; i++)
    {   
        // print information about sleeping process
        fprintf(file, "pid=%d prio=%d ('%s')\n", running_queue[i].pid, running_queue[i].prio, workload[running_queue[i].pid].cmd);
        *cpu_occupation -= running_queue[i].prio;
        fprintf(file, "CPU occupation: %d\n", *cpu_occupation);

        workload[running_queue[i].pid].idle++;
        workload[running_queue[i].pid].tf++;
        
        // insert running process into pending_queue
        pushProcess(pending_queue, running_queue[i], *pending_count);

        running_queue[i].pid = -1;
        running_queue[i].prio = -1;

        (*pending_count)++;
    }

    // decrease amount of process  in running queue
    int amountProcessesSlept = *running_count - newIndex_of_PendingProcess;
    *running_count -= amountProcessesSlept;
    
}


// main scheduler function
void schedule(workload_item *workload, int amountProcess, int CPU_size){
    queue_item running_queue[amountProcess];
    queue_item pending_queue[amountProcess];

    initializePendingQueue(workload, pending_queue, amountProcess);

    int running_count = 0;
    int pending_count = amountProcess;
    
    int timestep = 0;
    int cpu_occupation = 0;

    // Open the trace.txt in write mode, where trace if scheduler will be written
    FILE *file = fopen("trace.txt", "w");

    fprintf(file, "* starting scheduling on 1 CPUs");

    // iteration of each timestep in CPU
    while (timestep <= N)
    {   
        fprintf(file, "\n[t=%d]\n", timestep);
        // iterate over running queue, to find if there are any processes needed to finish
        for (int running_process_index = 0; running_process_index < running_count; running_process_index++)
        {   
            // check if it is time to finish process
            if (checkFinish(running_queue[running_process_index], workload, timestep))
            {   
                finishScenario(file, running_queue, &running_count, &running_process_index, workload, &cpu_occupation);
            }
            
        }
        
        // iterate over pending queue, to check situations of processes
        for (int pending_process_index = 0; pending_process_index < pending_count;) {
            queue_item pending_process = pending_queue[pending_process_index];
            int workload_process_index = pending_process.pid;

            // check if it is time to start process
            if (workload[workload_process_index].ts <= timestep) {
                // check if there is space in cpu to run process
                if ((cpu_occupation + pending_process.prio) <= CPU_size) {
                    runScenario_CPU_has_space(file, running_queue, &running_count, pending_queue, &pending_count, pending_process_index, &cpu_occupation, workload);

                // in case there is not enough space to run a process
                } else {
                    // get possibility of running process, and its possible index in running_queue
                    moveProcess check = checkProcessMove(running_queue, running_count, cpu_occupation, CPU_size, pending_process);
                    fprintf(file, "schedule pid=%d prio=%d ('%s') ... can't fit. ", pending_process.pid, pending_process.prio, workload[pending_process.pid].cmd);
                    fprintf(file, "Pick process to put asleep: ");
                    // if possible to run
                    if (check.possible)
                    {
                        handle_CPU_has_no_space_Higher_Priority(file, running_queue, &running_count, pending_queue, &pending_count, check, pending_process_index, &cpu_occupation, workload);
                        break;
                    }
                    else{
                        handle_CPU_has_no_space_Lower_Priority(file, running_queue, check, cpu_occupation, pending_process, workload);
                        break;
                    }
                    
                }
            } else {
                // increment only if no process was removed
                pending_process_index++;
            }
        }

        writeQueue(file, "running", running_queue, running_count);
        writeQueue(file, "pending", pending_queue, pending_count);

        timestep++;

    }

}

// Print final output that shows a recap of the scheduling for the input workload as a chronogram
void printFinalResult(workload_item *workload, int amountProcess){
    printf("\n        |....|....|....|....\n");
    
    for (int i = 0; i < amountProcess; i++)
    {
        printf("%s", workload[i].cmd);
        int length_of_cmd = strlen(workload[i].cmd);

        for (int j = 0; j < (8-length_of_cmd); j++)
        {
            printf(" ");
        }

        for (int j = 0; j < workload[i].ts; j++)
        {
            printf(" ");
        }

        int runTime = workload[i].tf - workload[i].ts;
        for (int j = 0; j < runTime; j++)
        {
            printf("X");
        }

        int timeLeft = 20 - workload[i].tf;
        for (int j = 0; j < timeLeft; j++)
        {
            printf(" ");
        }

        printf("                     ");
        printf("(tf=%ld, idle=%ld)\n", workload[i].tf, workload[i].idle);      
    

    }
    
}
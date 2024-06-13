// sched.h

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>


/**
 * @brief workload structure
 * 
 */
struct workload_item_t {
    int pid;            //< the event id
    int ppid;           //< the event parent id
    size_t ts;          //< start date
    size_t tf;          //< finish date
    size_t idle;        //< total time the process has been idle;
    char *cmd;          //< the binary name
    int prio;           //< process priority
};
typedef struct workload_item_t workload_item;

/**
 * @brief read source data of processes from @a filename
 * load process informations into @a workload array
 * increment @a amountProcess as mush as processes are read from @a filename
 * 
 * @param filename 
 * @param workload 
 * @param amountProcess 
 */
void read_sched_file(const char *filename, workload_item *workload, int *amountProcess);

/**
 * @brief queue item structure
 * 
 */
struct queue_item_t {
    int pid;            //< the event id
    int prio;           //< process priority
};
typedef struct queue_item_t queue_item;


/**
 * @brief check if it is time to finish a @a running_process, by comparing its finish time from @a workload array with @a timestep
 * 
 * @param running_process 
 * @param workload 
 * @param timestep 
 * @return true in case it is time to finish, false otherwise
 */
bool checkFinish(queue_item running_process, workload_item *workload, int timestep);


/**
 * @brief finish a process running located in @a running_queue in index @a running_process_index
 * 
 * @param running_queue 
 * @param running_process_index  represents amount of running processes in @a running_queue
 * @param running_count 
 */
void finishProcess(queue_item *running_queue, int running_process_index, int *running_count);


/**
 * @brief uses binary search to get index where the process - @a, movingProcess will be placed in new queue - @a q, where it is moving
 * 
 * @param q 
 * @param movingProcess 
 * @param process_count # represent amount of processes in the given queue - @a q
 * @return int index
 */
int binarySearch(queue_item *q, queue_item movingProcess, int process_count);


/**
 * @brief to run process @a pending_process move it to @a running_queue
 * 
 * @param running_queue 
 * @param running_count  represents amount of processes that exists in @a running_queue
 * @param pending_process 
 */
void runProcess(queue_item *running_queue, int *running_count, queue_item pending_process);


/**
 * @brief remove process, that runs in index @a pending_process_index, from @a pending_queue
 * 
 * @param pending_queue 
 * @param pending_process_index 
 * @param pending_count  represents amount of processes that exist in @a pending_queue
 */
void removeProcessFromPendingQueue(queue_item *pending_queue, int pending_process_index, int *pending_count);


/**
 * @brief struct that will be used to check if it is possible to run process or not in case of CPU is occupied
 * 
 */
struct moveProcess_t{
    int index;
    bool possible;
};
typedef struct moveProcess_t moveProcess;




/**
 * @brief Function to check if @a pending_process can be run in case cpu is occupied. 
 * To find out if cpu occupied compare priority values of @a pending_process with space left in CPU : calculate @a CPU_size - @a cpu_occupation
 * 
 * @param running_queue 
 * @param running_count   amount of processes that exist in @a running_queue
 * @param cpu_occupation 
 * @param CPU_size 
 * @param pending_process 
 * @return moveProcess - structure containing possible index of process in @a running_queue and bool value representing possiblity (True, False)
 */
moveProcess checkProcessMove(queue_item *running_queue, int running_count, int cpu_occupation, int CPU_size, queue_item pending_process);


/**
 * @brief sleep running process by moving it to @a pending_queue
 * Function writes situation of trace in @a file
 * Decrements @a running_count as much as processes are setted to sleep
 * Increments @a pending_count as mush as processes are setted to sleep
 * Decrements @a cpu_occupation as much as prios of processes that are setted to sleep
 * Increments tf and ts values of processes, that are setted to sleep, from @a workload
 * 
 * @param file 
 * @param running_queue 
 * @param running_count amount of processes that exist in @a running_queue
 * @param pending_queue 
 * @param pending_count amount of processes that exist in @a pending_queue
 * @param newIndex_of_PendingProcess 
 * @param workload 
 * @param cpu_occupation 
 */
void sleepRunningProcesses(FILE *file, queue_item *running_queue, int *running_count, queue_item *pending_queue, int *pending_count, int newIndex_of_PendingProcess, workload_item *workload, int *cpu_occupation);



/**
 * @brief final function that processes CPU scheduler
 * The function uses @a workload array to create pending and running queues to start scheduler process
 * The function uses @a CPU_size to limit amount of running processes
 * 
 * @param workload 
 * @param amountProcess  represent amount of processes that needed to be scheduled
 * @param CPU_size 
 */
void schedule(workload_item *workload, int amountProcess, int CPU_size);



/**
 * @brief Print final output that shows a recap of the scheduling for the input @a workload as a chronogram
 * 
 * @param workload 
 * @param amountProcess   represent amount of processes that scheduled
 */
void printFinalResult(workload_item *workload, int amountProcess);

#endif /* SCHEDULER_H */
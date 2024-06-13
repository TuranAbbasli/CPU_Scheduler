// scenario.h

#ifndef SCENARIO_H
#define SCENARIO_H

#include "scheduler.h"
#include "write.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>


/**
 * @brief The function writes a finishing statues message in @a file by getting process informations from @a workload
 * Removes process from @a running_queue, then decrements @a running_count
 * To keep iteration on @a running_queue working succesfully @a running_process_index is decremented
 * As a process is stoped working, @a cpu_occupation is reduced
 * 
 * @param file 
 * @param running_queue 
 * @param running_count  represents amount of processes that exists in @a running_queue
 * @param running_process_index 
 * @param workload 
 * @param cpu_occupation 
 */
void finishScenario(FILE *file, queue_item *running_queue, int *running_count, int *running_process_index, workload_item *workload, int *cpu_occupation);



/**
 * @brief The function runs process in @a pending_process_index from @a pending_queue by moving it into @a running_queue
 * Writes trace status in @a file by getting process informations from @a workload
 * Removes process @a pending_process_index from @a pending_queue
 * As process in @a pending_process_index from @a pending_queue is added into @a running_queue, @a cpu_occupation is increased
 * 
 * @param file 
 * @param running_queue 
 * @param running_count  represents amount of processes that exists in @a running_queue
 * @param pending_queue 
 * @param pending_count  represents amount of processes that exists in @a pending_queue
 * @param pending_process 
 * @param pending_process_index 
 * @param cpu_occupation 
 * @param workload 
 */
void runScenario_CPU_has_space(FILE *file, queue_item *running_queue, int *running_count, queue_item *pending_queue, int *pending_count,
                               int pending_process_index, int *cpu_occupation, workload_item *workload);



/**
 * @brief The function runs process in @a pending_process_index from @a pending_queue by moving it into @a running_queue
 * As CPU has not enough space, and it is Higher_Priority scenario, which is decided by @a check, lower priority processes in @a running_queue are setted to sleep.
 * After making space in CPU, process in @a pending_process_index from @a pending_queue is pushed into @a running_queue .
 * Process that started to running is removed from @a pending_queue .
 * As process in @a pending_process_index from @a pending_queue is added into @a running_queue, @a cpu_occupation is increased
 * Writes trace status in @a file by getting process informations from @a workload
 * 
 * @param file 
 * @param running_queue 
 * @param running_count  represents amount of processes that exists in @a running_queue
 * @param pending_queue 
 * @param pending_count  represents amount of processes that exists in @a pending_queue
 * @param check 
 * @param pending_process 
 * @param pending_process_index 
 * @param cpu_occupation 
 * @param workload 
 */
void handle_CPU_has_no_space_Higher_Priority(FILE *file, queue_item *running_queue, int *running_count, queue_item *pending_queue, int *pending_count,
                                             moveProcess check, int pending_process_index, int *cpu_occupation, workload_item *workload);



/**
 * @brief The function runs process in @a pending_process_index from @a pending_queue by moving it into @a running_queue
 * As CPU has not enough space, and it is Lower_Priority scenario, which is decided by @a check, no movements of processes are done.
 * Idle and tf values from @a workload is incremented for the process that cannot run, as it's prio is not high enough
 * Writes trace status in @a file by getting process informations from @a workload
 * 
 * @param file 
 * @param running_queue 
 * @param check 
 * @param cpu_occupation 
 * @param pending_process 
 * @param workload 
 */
void handle_CPU_has_no_space_Lower_Priority(FILE *file, queue_item *running_queue, moveProcess check, 
                                            int cpu_occupation, queue_item pending_process, workload_item *workload);


#endif /* SCENARIO_H */
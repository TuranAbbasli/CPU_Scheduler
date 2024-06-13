// write.h

#ifndef WRITE_H
#define WRITE_H

#include "scheduler.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>


/**
 * @brief Write Queue @a q  and its name @a q_name in txt @a file
 * 
 * @param file 
 * @param q_name 
 * @param q 
 * @param q_count represents amount of processes that exist in queue @a q
 */
void writeQueue(FILE *file, char *q_name, queue_item *q, int q_count);


/**
 * @brief Write a message in @a file about process that finished
 * The process that will stop is in @a running_process_index in @a running_queue
 * Uses @a workload to get more detailed information about the process
 * 
 * @param file 
 * @param running_queue 
 * @param running_process_index 
 * @param workload 
 */
void writeFInishMessage(FILE *file, queue_item *running_queue, int running_process_index, workload_item *workload);

#endif /* WRITE_H */
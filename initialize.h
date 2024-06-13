// initialize.h

#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "scheduler.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief swap a and b
 * 
 * @param a 
 * @param b 
 */
void swap(queue_item* a, queue_item* b);


/**
 * @brief partition function used by quicksort to rearrange elements based on a pivot
 * 
 * @param arr 
 * @param low 
 * @param high 
 * @return int 
 */
int partition(queue_item arr[], int low, int high);


/**
 * @brief quicksort function to recursively sort an array of queue_item structures based on their priority (prio)
 * 
 * @param arr 
 * @param low 
 * @param high 
 */
void quickSort(queue_item arr[], int low, int high);



/**
 * @brief sortPendingQueue function to apply quicksort on the @a pending_queue
 * @a amoutProcess represent count of processes in the @a pending_queue
 * 
 * @param pending_queue 
 * @param amountProcess 
 */
void sortPendingQueue(queue_item *pending_queue, int amountProcess);

/**
 * @brief initialize @a pending_queue by loading processes in @a workload array
 * @a amoutProcess represent count of processes that will be loaded
 * 
 * @param workload 
 * @param pending_queue 
 * @param amountProcess 
 */
void initializePendingQueue(workload_item *workload, queue_item *pending_queue, int amountProcess);


#endif /* INTIALIZE_H */
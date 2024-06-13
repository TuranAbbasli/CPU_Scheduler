#include "scheduler.h"

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

// swap function for efficiently exchanging two queue_item structures
void swap(queue_item* a, queue_item* b) {
    queue_item temp = *a;
    *a = *b;
    *b = temp;
}


// partition function used by quicksort to rearrange elements based on a pivot
int partition(queue_item arr[], int low, int high) {
    int pivot = arr[high].prio;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].prio > pivot) {  // Change comparison for descending order
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}


// quicksort function to recursively sort an array of queue_item structures based on their priority (prio)
void quickSort(queue_item arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


// sortPendingQueue function to apply quicksort on the pending queue
void sortPendingQueue(queue_item *pending_queue, int amountProcess) {
    quickSort(pending_queue, 0, amountProcess - 1);
}

// initializePendingQueue function to populate and sort the pending queue
void initializePendingQueue(workload_item *workload, queue_item *pending_queue, int amountProcess){
    for (int i = 0; i < amountProcess; i++)
    {
        pending_queue[i].pid = workload[i].pid;
        pending_queue[i].prio = workload[i].prio;
    }

    sortPendingQueue(pending_queue, amountProcess); 
    
}
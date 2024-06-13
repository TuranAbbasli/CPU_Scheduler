# CPU_scheduler
A CPU scheduler that runs tasks by their priorities using Data Structures in C

The scheduler initally reads tasks from sched.txt file, then creates 2 priority queues to run those tasks by moving them between those queues which are pending_queue and running_queue. The maximum capacity of CPU is 20 which means that sum of process priorities in running queue can not exceed this limit.

After creating both queues, all processes in the workload are loaded into the pending queue, and those processes are sorted using the `initialize` function which has a complexity of O(n log n) as it uses Quick Sort to sort the queue by priority values of processes.

Then the scheduler starts to work on processes in a time length of 20.

- During this iteration, the first running queue is checked to finish any processes using the function `finishScenario()`, so the total complexity of checking and finishing processes is O(n).

- After making space on the CPU, iteration starts on the pending queue. If any process needs to start:
  - If there is enough space, the process is dequeued from the pending queue and pushed into the running queue. Both processes' complexity is O(n), and using `limit runScenario_CPU_has_space()` will have a complexity of O(n).
  - In case the CPU does not have enough space and the process's priority is high enough to run, lower priority processes are settled to sleep, the pending process is dequeued, and pushed into the running queue. The total `handle_CPU_has_no_space_Higher_Priority()` will have a complexity of O(n), as each function used has O(n) complexity.
  - In case the CPU does not have enough space and the process's priority is not high enough to run, no processes are moved. Only the status of those processes is updated, so that function `handle_CPU_has_no_space_Lower_Priority()` has O(1) complexity.

To sum up all scenarios and functions mentioned above, it can be concluded that the scheduler function has approximately a time complexity of O(n^2) if clock time iteration is considered as O(1), as N=20 is a constant.

#include <omp.h>
#include <stdio.h>

void simpleTask(int taskID) { printf("Task %d started\n", taskID); }

int main() {
#pragma omp parallel
  {
#pragma omp single
    {
// Creating tasks
#pragma omp task
      simpleTask(1);

#pragma omp task
      simpleTask(2);

// Taskwait directive to ensure all tasks are complete
#pragma omp taskwait

      // This will only execute after both tasks 1 and 2 are completed
      printf("All tasks are complete.\n");
    }
  }

  return 0;
}

#include <omp.h>
#include <stdio.h>

// here Two gets printed twice, One and Three just get printed once
int main(int argc, char const *argv[]) {

#pragma omp parallel num_threads(2)
  {
#pragma omp single
    printf("One \n");

    printf("Two \n");

#pragma omp single
    printf("Three \n");
  }
  return 0;
}

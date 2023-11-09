#include <omp.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
#pragma omp parallel
  { printf("Hello World\n"); }
  return 0;
}

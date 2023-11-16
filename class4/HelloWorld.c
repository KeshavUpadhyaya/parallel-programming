#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {

  int size = 0;

  // MUST BE CALLED at first
  MPI_Init(NULL, NULL);
  printf("Hello World");

  // MUST BE CALLED at the end
  MPI_Finalize();

  return 0;
}
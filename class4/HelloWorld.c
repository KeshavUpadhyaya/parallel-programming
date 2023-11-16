#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

// run with mpirun -n 4 --hostfile hostfile.txt --pernode ./a.out  for running
// in 4 different machines

int main(int argc, char **argv) {

  int size = 0, rank = 0;
  char hn[128];
  gethostname(hn, 128);

  // MUST BE CALLED at first
  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  printf("Hello World from %s. I am node %d of %d \n", hn, rank, size);

  // MUST BE CALLED at the end
  MPI_Finalize();

  return 0;
}

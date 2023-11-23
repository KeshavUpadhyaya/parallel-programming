#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

// SSH into a linux lab machine & run in order to have access to other
// machines.
// Run with mpirun -n 4 --hostfile hostfile.txt --pernode ./a.out  for
// execution in 4 different machines

int main(int argc, char **argv) {

  // MUST BE CALLED at first
  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int number;
  if (world_rank == 0) {
    number = -1;
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } else if (world_rank == 1) {
    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 1 received number %d from process 0\n", number);
  }

  // MUST BE CALLED at the end
  MPI_Finalize();
}

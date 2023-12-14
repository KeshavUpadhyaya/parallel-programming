#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
  MPI_Init(0, 0);

  int size = 0, rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int local_n, n = 0;
  local_n = rank;
  int recvRank;

  if (rank == 0) {
    // add partSum of node 0
    n += local_n;
    for (int i = 1; i < size; i++) {

      MPI_Recv(&recvRank, 1, MPI_INT, i, 1, MPI_COMM_WORLD,
               MPI_STATUSES_IGNORE);
      n += local_n;
    }

    printf("sum of all local_n : %d\n", n);

  } else {
    // send data
    // MPI_Ssend is better since MPI_Send is implementation dependent
    MPI_Ssend(&local_n, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}

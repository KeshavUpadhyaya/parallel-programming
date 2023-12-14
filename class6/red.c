#include <mpi.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  MPI_Init(0, 0);

  int size = 0, rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int local_n, n;
  local_n = rank;

  MPI_Reduce(&local_n /*send_buf*/, &n /*recv_buf*/, 1 /*count*/, MPI_INT,
             MPI_SUM, 0 /*dest_process*/, MPI_COMM_WORLD);
  printf("sum of all local_n : %d", n);

  MPI_Finalize();
  return 0;
}

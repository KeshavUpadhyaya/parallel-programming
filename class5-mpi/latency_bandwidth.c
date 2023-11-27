#include <mpi.h>
#include <stdio.h>

#define TAG_PING 17
#define TAG_PONG 23

void test_latency(int rank, int size, int N) {
  double start_time, end_time, total_latency = 0.0;
  MPI_Status status;
  float data = 1.7;

  if (rank == 0) {
    for (int i = 0; i < N; i++) {
      start_time = MPI_Wtime();
      MPI_Ssend(&data, 1, MPI_FLOAT, 1, TAG_PING, MPI_COMM_WORLD);
      MPI_Recv(&data, 1, MPI_FLOAT, 1, TAG_PONG, MPI_COMM_WORLD, &status);
      end_time = MPI_Wtime();
      total_latency += (end_time - start_time);
    }
  } else if (rank == 1) {
    for (int i = 0; i < N; i++) {
      MPI_Recv(&data, 1, MPI_FLOAT, 0, TAG_PING, MPI_COMM_WORLD, &status);
      MPI_Ssend(&data, 1, MPI_FLOAT, 0, TAG_PONG, MPI_COMM_WORLD);
    }
  }

  printf("total latency = %lf\n", total_latency);
}

int main(int argc, char const *argv[]) {
  MPI_Init(NULL, NULL);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int N = 1000;
  test_latency(rank, size, N);

  MPI_Finalize();
  return 0;
}

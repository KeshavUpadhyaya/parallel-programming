#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAG_PING 17
#define TAG_PONG 23
#define N 10

void test_latency(int rank, int size) {
  double start_time, end_time, total_latency = 0.0;
  MPI_Status status;
  float data = 1.7;

  if (rank == 0) {
    start_time = MPI_Wtime();
    for (int i = 0; i < N; i++) {
      MPI_Ssend(&data, 1, MPI_FLOAT, 1, TAG_PING, MPI_COMM_WORLD);
      MPI_Recv(&data, 1, MPI_FLOAT, 1, TAG_PONG, MPI_COMM_WORLD, &status);
    }
    end_time = MPI_Wtime();
    total_latency += (end_time - start_time);

    printf("Latency for sending floats: %lf ns\n",
           total_latency / (2 * N) * 1E9);

  } else if (rank == 1) {
    for (int i = 0; i < N; i++) {
      MPI_Recv(&data, 1, MPI_FLOAT, 0, TAG_PING, MPI_COMM_WORLD, &status);
      MPI_Ssend(&data, 1, MPI_FLOAT, 0, TAG_PONG, MPI_COMM_WORLD);
    }
  }
}

void test_bandwidth(int rank, int size) {
  double start_time, end_time;
  MPI_Status status;

  int startSize = 1024;           //  1 KB
  int maxSize = 10 * 1024 * 1024; // 10 MB
  char *message = (char *)malloc(maxSize);

  if (rank == 0) {
    printf("Computing bandwidth for various buffer sizes...\n");
    printf("| Size (bytes) | Bandwidth (GB/s) |\n");
    printf("|--------------:|------------------:|\n");
    for (int j = startSize; j < maxSize; j = j * 2) {

      start_time = MPI_Wtime();
      for (int i = 0; i < N; i++) {
        MPI_Ssend(message, j, MPI_CHAR, 1, TAG_PING, MPI_COMM_WORLD);
      }

      end_time = MPI_Wtime();

      double latency = (end_time - start_time);
      double bandwidth = j * N / (end_time - start_time);

      printf("| %13d | %16lf |\n", j, (bandwidth / (1024 * 1024 * 1024)));
    }

  } else if (rank == 1) {
    for (int j = startSize; j < maxSize; j = j * 2) {

      for (int i = 0; i < N; i++) {
        MPI_Recv(message, j, MPI_CHAR, 0, TAG_PING, MPI_COMM_WORLD, &status);
      }
    }
  }
  free(message); // freeing after use
}

int main(int argc, char const *argv[]) {
  MPI_Init(NULL, NULL);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  test_latency(rank, size);

  test_bandwidth(rank, size);

  MPI_Finalize();
  return 0;
}

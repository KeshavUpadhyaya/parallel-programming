#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TAG_PING 17
#define TAG_PONG 23

void test_latency(int rank, int size, int N) {
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

    printf("Latency: %lf ns\n", total_latency / (2 * N) * 1E9);

  } else if (rank == 1) {
    for (int i = 0; i < N; i++) {
      MPI_Recv(&data, 1, MPI_FLOAT, 0, TAG_PING, MPI_COMM_WORLD, &status);
      MPI_Ssend(&data, 1, MPI_FLOAT, 0, TAG_PONG, MPI_COMM_WORLD);
    }
  }
}

void test_bandwidth(int rank, int size, int message_size) {
  double start_time, end_time;
  MPI_Status status;

  char *message = (char *)malloc(message_size);

  if (rank == 0) {
    start_time = MPI_Wtime();
    MPI_Send(message, message_size, MPI_CHAR, 1, TAG_PING, MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    double latency = (end_time - start_time) / 2 * 1E9;
    double bandwidth = message_size / (end_time - start_time);
    printf("Bandwidth: %lf MB/s\n", bandwidth / (1024 * 1024));

  } else if (rank == 1) {
    MPI_Recv(message, message_size, MPI_CHAR, 0, TAG_PING, MPI_COMM_WORLD,
             &status);
  }

  free(message); // freeing after use
}

int main(int argc, char const *argv[]) {
  MPI_Init(NULL, NULL);

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int N = 1000;
  test_latency(rank, size, N);

  int message_size = 1024 * 1024 * 10; // 10MB
  test_bandwidth(rank, size, message_size);

  MPI_Finalize();
  return 0;
}


#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

#define f(A) (4.0 / (1.0 + A * A))

const long long n = 10000000000;

double x, pi;
double w = 1.0 / n;
double sum = 0.0;

double partPiFnc(long long start, long long end) {
  double sum = 0.0;
  for (long long i = start; i < end; i++) {
    x = w * ((double)i + 0.5);
    sum = sum + f(x);
  }
  return sum;
}

int main(int argc, char *argv[]) {

  int size = 0, rank = 0;
  char hn[128];
  gethostname(hn, 128);

  // MUST BE CALLED at first
  MPI_Init(NULL, NULL);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  long long stepWidth = n / size;

  // calculate start, end
  long long start = rank * stepWidth;
  long long end = (rank + 1) * stepWidth;

  // calculate part sum
  double partSum = partPiFnc(start, end);

  printf("pi part from rank %d = %lf", rank, partSum);
  double receivePartSum = 0;

  if (rank == 0) {
    // receive data
    for (int i = 0; i < size; i++) {
      MPI_Recv(&receivePartSum, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD,
               MPI_STATUSES_IGNORE);
      printf("Received part sum %lf from %d\n", receivePartSum, i);
      sum += receivePartSum;
    }
  } else {
    // send data
    MPI_Send(&partSum, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }

  printf("pi = %.15f\n", w * sum);
  return 0;
}

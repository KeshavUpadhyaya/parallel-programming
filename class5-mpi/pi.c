
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

// mpirun -n 512 --npernode 24 --oversubscribe --hostfile hostfile.txt ./a.out
// it creates 512 processes and 24 processes per node
// you need to have atleast 512/24 processes per node

#define f(A) (4.0 / (1.0 + A * A))

const long long n = 10000000000;

double x, pi;
double w = 1.0 / n;
double finalSum = 0.0;

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

  double receivePartSum = 0;

  // consider 0 as the main node
  if (rank == 0) {
    // add partSum of node 0
    finalSum += partSum;
    // ignore 0 - don't receive from itself
    for (int i = 1; i < size; i++) {
      // receive data - the below function is a blocking function so it waits to
      // recieve
      MPI_Recv(&receivePartSum, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD,
               MPI_STATUSES_IGNORE);

      // You can use MPI_ANY_SOURCE to get from any node instead of receiving
      // from specific ones - MUCH FASTER!!
      //  MPI_Recv(&receivePartSum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1,
      //  MPI_COMM_WORLD,     MPI_STATUSES_IGNORE);
      finalSum += receivePartSum;
    }

    printf("pi = %.15f\n", w * finalSum);

  } else {
    // send data
    MPI_Send(&partSum, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
  }

  // MUST BE CALLED at the end
  MPI_Finalize();

  return 0;
}

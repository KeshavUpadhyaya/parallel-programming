#include <omp.h>
#include <stdio.h>

#define f(A) (4.0 / (1.0 + A * A))
#define NUM_OF_THREADS 8

const long long n = 1000000000;
long long i;
double x, pi;
double w = 1.0 / n;

// better solution for distribution
// each thread has struct which contains partial sum
// in the end we are summing partial sums

typedef struct {
  long long start;
  long long end;
  double partSolution;
} input_t;

double partPiFnc(long long start, long long end) {
  double sum = 0.0;
  for (long long i = start; i < end; i++) {
    x = w * ((double)i + 0.5);
    sum = sum + f(x);
  }
  return sum;
}

int main(int argc, char *argv[]) {
  double partResult[NUM_OF_THREADS];
  double sum = 0.0;
  long long stepWidth = n / NUM_OF_THREADS;

#pragma omp parallel
  {
#pragma omp sections
    {
#pragma omp section
      partResult[0] = partPiFnc(0 * stepWidth, 1 * stepWidth);

#pragma omp section
      partResult[1] = partPiFnc(1 * stepWidth, 2 * stepWidth);

#pragma omp section
      partResult[2] = partPiFnc(2 * stepWidth, 3 * stepWidth);

#pragma omp section
      partResult[3] = partPiFnc(3 * stepWidth, 4 * stepWidth);

#pragma omp section
      partResult[4] = partPiFnc(4 * stepWidth, 5 * stepWidth);

#pragma omp section
      partResult[5] = partPiFnc(5 * stepWidth, 6 * stepWidth);

#pragma omp section
      partResult[6] = partPiFnc(6 * stepWidth, 7 * stepWidth);

#pragma omp section
      partResult[7] = partPiFnc(7 * stepWidth, 8 * stepWidth);
    }
  }

  for (int j = 0; j < NUM_OF_THREADS; j++) sum += partResult[j];

  printf("pi = %.15f\n", w * sum);
  return 0;
}

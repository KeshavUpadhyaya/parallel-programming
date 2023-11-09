// Using the critical directive with pi so that the correct answer is generated

// Faster do do with single thread with multiple sections

#include <omp.h>
#include <stdio.h>

#define f(A) (4.0 / (1.0 + A * A))

const long long n = 10000000000;

int main(int argc, char *argv[]) {
  long long i;
  double x, sum, pi;
  double w = 1.0 / n;
  sum = 0.0;

#pragma omp parallel for
  for (i = 0; i < n; i++) {
    x = w * ((double)i + 0.5);

#pragma omp critical
    sum = sum + f(x);
  }

  printf("pi = %.15f\n", w * sum);
  return 0;
}

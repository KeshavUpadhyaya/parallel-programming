// Using reduction to compute pi - works fine!

#include <omp.h>
#include <stdio.h>

#define f(A) (4.0 / (1.0 + A * A))

const long long n = 10000000000;

int main(int argc, char *argv[]) {
  long long i;
  double x, sum, pi;
  double w = 1.0 / n;
  sum = 0.0;

#pragma omp parallel for reduction(+ : sum)
  for (i = 0; i < n; i++) {
    x = w * ((double)i + 0.5);
    sum = sum + f(x);
  }

  printf("pi = %.15f\n", w * sum);
  return 0;
}

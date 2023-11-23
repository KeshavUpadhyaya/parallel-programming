#include <stdio.h>

#define f(A) (4.0 / (1.0 + A * A))

const long long n = 10000000000;

double x, sum, pi;
double w = 1.0 / n;
sum = 0.0;

double partPiFnc(long long start, long long end) {
  double sum = 0.0;
  for (long long i = start; i < end; i++) {
    x = w * ((double)i + 0.5);
    sum = sum + f(x);
  }
  return sum;
}

int main(int argc, char *argv[]) {
  long long i;
  double x, sum, pi;
  double w = 1.0 / n;
  sum = 0.0;

  for (i = 0; i < n; i++) {
    x = w * ((double)i + 0.5);
    sum = sum + f(x);
  }

  printf("pi = %.15f\n", w * sum);
  return 0;
}

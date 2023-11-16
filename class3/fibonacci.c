#include <omp.h>
#include <stdio.h>

int fibo(int n) {
  int x, y;

  if (n < 2)
    return n;

#pragma omp task shared(x) if (n > 30)
  x = fibo(n - 1);

#pragma omp task shared(y) if (n > 30)
  y = fibo(n - 2);

#pragma omp taskwait
  return x + y;
}

int main() {
  int NN = 1;

#pragma omp parallel
  {

#pragma omp master
    fibo(NN);
  }
}

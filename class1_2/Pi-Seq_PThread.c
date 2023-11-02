#include <pthread.h>
#include <stdio.h>

#define f(A) (4.0 / (1.0 + A * A))
#define NUM_OF_THREADS 4

const long long n = 10000000000;
long long i;
double x, pi;
double w = 1.0 / n;

typedef struct {
  // in
  long long start;
  long long end;
  // out
  double partSolution;
} input_t;

void *piFunction(void *_data) {
  double sum = 0.0;
  input_t *inout = (input_t *)_data;
  for (i = inout->start; i < inout->end; i++) {
    x = w * ((double)i + 0.5);
    sum = sum + f(x);
  }
  inout->partSolution = sum;
}

int main(int argc, char *argv[]) {
  pthread_t thread[NUM_OF_THREADS];  // Array of threads
  input_t
      in[NUM_OF_THREADS];  // create for every thread an array entry with its
                           // own struct to avoid overriding by other thread
  long stepWidth = n / NUM_OF_THREADS;
  for (int j = 0; j < NUM_OF_THREADS; j++) {
    in[j].start = j * stepWidth;
    in[j].end = (j + 1) * stepWidth;
    pthread_create(&thread[j], NULL, piFunction, &in[j]);
  }

  for (int j = 0; j < NUM_OF_THREADS; j++) pthread_join(thread[j], NULL);

  double sum = 0.0;
  for (int j = 0; j < NUM_OF_THREADS; j++) sum += in[j].partSolution;

  printf("pi = %.15f\n", w * sum);
  return 0;
}

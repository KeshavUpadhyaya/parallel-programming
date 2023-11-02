#include <stdio.h>
#include <pthread.h>

#define f(A) (4.0/(1.0 + A*A))
#define NUM_OF_THREADS 8

const long long n = 1000000000;
long long i;
double x, pi;
double w = 1.0/n;

//better solution for distribution
//each thread has struct which contains partial sum
//in the end we are summing partial sums

typedef struct {
  long long start;
  long long end;
  double partSolution;
} input_t;

void* piFunction(void* _data){
  double sum = 0.0;
  input_t* inout  = (input_t*) _data;
  for (long long i=inout->start; i<inout->end; i++){
    x = w * ((double)i + 0.5);
    sum = sum + f(x);
  }
  inout->partSolution = sum;
  return NULL;
}

int main(int argc, char* argv[]) {
  //create threads and structs (same number of)
  pthread_t thread[NUM_OF_THREADS];
  input_t in[NUM_OF_THREADS];
  int stepWidth = n / NUM_OF_THREADS;

  
  for (int j = 0;  j < NUM_OF_THREADS; j++){
    in[j].start = j * stepWidth;
    in[j].end = (j+1) * stepWidth;
    pthread_create(&thread[j], NULL, piFunction, &in[j]);
  }

  for (int j = 0;  j < NUM_OF_THREADS; j++)
    pthread_join(thread[j], NULL);

  double sum = 0.0;

  for (int j = 0;  j < NUM_OF_THREADS; j++)
    sum+=in[j].partSolution;

  printf("pi = %.15f\n", w * sum);
  return 0;
}

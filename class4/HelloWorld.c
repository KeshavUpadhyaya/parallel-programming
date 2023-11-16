#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {

  int size = 0;
  char hn[128];
  gethostname(hn, 128);

  // MUST BE CALLED at first
  MPI_Init(NULL, NULL);
  printf("Hello World from %s \n", hn);

  // MUST BE CALLED at the end
  MPI_Finalize();

  return 0;
}
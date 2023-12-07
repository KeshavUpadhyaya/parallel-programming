#include <mpi.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  MPI_Init(0, 0);

  int wRank, wSize;
  int sRank = -1, sSize = -1;

  MPI_Comm_size(MPI_COMM_WORLD, &wSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &wRank);

  printf("World Size: %d World rank : %d\n", &wSize, &wRank);
  printf("Sub size: %d Sub rank: %d", &sSize, &sRank);

  MPI_Finalize();
  return 0;
}

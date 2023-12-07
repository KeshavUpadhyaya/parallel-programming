#include <mpi.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  MPI_Init(0, 0);

  int wRank, wSize;
  int sRank = -1, sSize = -1;

  MPI_Comm_size(MPI_COMM_WORLD, &wSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &wRank);

  MPI_Comm subComm;
  int color = wRank % 3;
  MPI_Comm_split(MPI_COMM_WORLD, color, 0, &subComm);

  MPI_Comm_size(subComm, &sSize);
  MPI_Comm_rank(subComm, &sRank);

  printf(
      "World Size: %d World rank : %d Color: %d Sub size: %d Sub rank: %d \n",
      wSize, wRank, color, sSize, sRank);

  MPI_Finalize();
  return 0;
}

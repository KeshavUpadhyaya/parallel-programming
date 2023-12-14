#include <mpi.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  MPI_Init(0, 0);

  int wRank, wSize;
  int sRank1 = -1, sSize1 = -1;
  int sRank2 = -1, sSize2 = -1;

  MPI_Comm_size(MPI_COMM_WORLD, &wSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &wRank);

  int buf;

  MPI_Comm subComm1;
  int color = wRank % 3;

  MPI_Comm_split(MPI_COMM_WORLD, color, 0,
                 &subComm1); // key is 0 ascending order of sub-ranks

  MPI_Comm_size(subComm1, &sSize1);
  MPI_Comm_rank(subComm1, &sRank1);

  MPI_Comm subComm2;

  MPI_Comm_split(MPI_COMM_WORLD, color, 1,
                 &subComm2); // key is 0 ascending order of sub-ranks

  MPI_Comm_size(subComm2, &sSize2);
  MPI_Comm_rank(subComm2, &sRank2);

  printf("World Size: %d World rank : %d Color: %d Sub1 size: %d Sub1 rank: %d "
         "Sub2 size: %d Sub2 rank: %d\n",
         wSize, wRank, color, sSize1, sRank1, sSize2, sRank2);

  MPI_Finalize();
  return 0;
}

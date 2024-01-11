
/************************************************************************
 *
 * This file has been written as a sample solution to an exercise in a
 * course given at the HLRS www.hlrs.de . It is made
 * freely available with the understanding that every copy of this file
 * must include this header and that HLRS takes no responsibility for
 * the use of the enclosed teaching material.
 *
 * Authors:    Rolf Rabenseifner
 *
 * Contact:    rabenseifner@hlrs.de
 *
 * Purpose:    A program to test parallel file I/O with MPI.
 *
 * Contents:   C source code.
 *
 ***********************************************************************/

#include <mpi.h>
#include <stdio.h>
// parallel writing
void main(int argc, char *argv[]) {
  int my_rank, size, i;

  int ndims, array_of_sizes[1], array_of_subsizes[1];
  int array_of_starts[1], order;
  MPI_File fh;
  MPI_Offset offset;
  MPI_Status status;

  char buf;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_File_open(MPI_COMM_WORLD, "my_test_file",
                MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

  for (i = 0; i < 5; i++) {
    buf = 'a' + my_rank;
    // output will be abcdabcdabcdabcdabcd if you run for 4 nodes
    offset = size;
    MPI_File_write_at(fh, my_rank + (offset * i), &buf, 1, MPI_CHAR, &status);
  }

  MPI_File_close(&fh);

  printf("PE%d\n", my_rank);

  MPI_Finalize();

  /*
    Note: all nodes write to the same file
    Offset explanation for when it runs on the first node:
    Assume size = 4
    So 4 nodes: 0 1 2 3

    first node: my_rank = 0
    => buf = 'a'+ rank =  'a' + 0 = 'a'
    i = 0
    0 + (4 * 0) = 0 -> 'a' gets written on 0th place

    i = 1
    0 + (4 * 1) = 4 -> 'a' gets written on 4th place

    i = 2
    0 + (4 * 2) = 8 -> 'a' gets written on 8th place

    i = 3
    0 + (4 * 3) = 12 -> 'a' gets written on 12th place

    i = 4
    0 + (4 * 4) = 16-> 'a' gets written on 16th place
  */
}

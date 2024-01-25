
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

void main(int argc, char *argv[]) {
  int my_rank, size, i;

  int ndims, array_of_sizes[1], array_of_subsizes[1];
  int array_of_starts[1], order;
  MPI_File fh;
  MPI_Datatype etype;
  MPI_Datatype filetype;
  int disp;
  MPI_Status status;

  char buf;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  etype = MPI_CHAR;
  ndims = 1;
  array_of_sizes[0] = size;
  array_of_subsizes[0] = 1;
  array_of_starts[0] = my_rank;
  order = MPI_ORDER_C;

  /*
    TLDR:
    creating custom data type and setting view
    Not required to compute offset manually

    Advantages of Modified Version:
    Flexibility with Data Layout:
    The use of MPI derived data types and file views allows you to describe more
    complex data layouts, including non-contiguous and irregular patterns. This
    can be beneficial when dealing with multidimensional arrays or structures.
    Simplified Code:

    The modified version eliminates the manual calculation of offsets for each
    process. The derived datatype and file view abstract away these details,
    resulting in cleaner and more readable code. Easier Maintenance:

    If the data layout changes, updating the MPI derived data type and file view
    is often simpler than modifying multiple instances of offset calculations.
    Portability:

    The use of MPI derived data types can improve the portability of your code
    across different MPI implementations and architectures. Considerations:
    Complexity:

    While the modified version provides more abstraction, it may introduce a
    level of complexity that is not necessary for simple applications. The
    choice between the two approaches depends on the requirements of your
    specific use case. Performance:

    In some cases, manually calculating offsets for individual processes might
    be more efficient than using derived data types and file views. Performance
    considerations should be evaluated based on the characteristics of your
    application and the parallel file system.
  */

  MPI_Type_create_subarray(ndims, array_of_sizes, array_of_subsizes,
                           array_of_starts, order, etype, &filetype);
  MPI_Type_commit(&filetype);

  MPI_File_open(MPI_COMM_WORLD, "my_test_file",
                MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

  disp = 0;
  MPI_File_set_view(fh, disp, etype, filetype, "native", MPI_INFO_NULL);

  for (i = 0; i < 5; i++) {
    buf = 'a' + (char)my_rank;
    MPI_File_write(fh, &buf, 1, etype, &status);
  }

  MPI_File_close(&fh);

  printf("PE%d\n", my_rank);

  MPI_Finalize();
}
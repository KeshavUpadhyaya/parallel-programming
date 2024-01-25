#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MATRIX_SIZE 8000

void printMatrix(double **arr) {

  for (int i = 0; i < MATRIX_SIZE; i++) {

    for (int j = 0; j < MATRIX_SIZE; j++) {

      printf("%.2f ", arr[i][j]);
    }

    printf("\n");
  }

  printf("\n");
}

// fill matrix with random numbers

void fillArray(double **arr) {

  for (int i = 0; i < MATRIX_SIZE; i++) {

    for (int j = 0; j < MATRIX_SIZE; j++) {

      int random = rand();

      arr[i][j] = (double)random / RAND_MAX;
    }
  }
}

// multiply one row of matrix_a with matrix_b and store solution

void multiply_matrices(double *buffer, double *answer, double **arr) {

  for (int i = 0; i < MATRIX_SIZE; i++) {

    answer[i] = 0;

    for (int j = 0; j < MATRIX_SIZE; j++) {

      answer[i] += buffer[j] * arr[j][i];
    }
  }
}

int main(int argc, char *argv[]) {

  srand(time(0));

  double **matrix_a;

  double **matrix_b;

  double **matrix_c;

  double *buffer;

  double *answer;

  matrix_b = (double **)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double *));

  for (int i = 0; i < MATRIX_SIZE; i++) {

    matrix_b[i] = (double *)malloc(MATRIX_SIZE * sizeof(double));
  }

  answer = (double *)malloc(MATRIX_SIZE * sizeof(double));

  int rank;

  int size;

  int ierr;

  // printf("test\n");

  MPI_Status status;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {

    matrix_a = (double **)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double *));

    matrix_c = (double **)malloc(MATRIX_SIZE * MATRIX_SIZE * sizeof(double *));

    for (int i = 0; i < MATRIX_SIZE; i++) {

      matrix_a[i] = (double *)malloc(MATRIX_SIZE * sizeof(double));

      matrix_c[i] = (double *)malloc(MATRIX_SIZE * sizeof(double));
    }

    fillArray(matrix_a);

    fillArray(matrix_b);

    /*printf("matrix_a\n");

    printMatrix(matrix_a);

    printf("matrix_b\n");

    printMatrix(matrix_b);*/

    for (int i = 0; i < MATRIX_SIZE; i++) {

      ierr = MPI_Bcast(matrix_b[i], MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    int row_a_counter = 0;

    for (int i = 1; i < size; i++) {

      ierr = MPI_Send(matrix_a[i - 1], MATRIX_SIZE, MPI_DOUBLE, i, i,
                      MPI_COMM_WORLD);

      row_a_counter++;
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {

      ierr = MPI_Recv(answer, MATRIX_SIZE, MPI_DOUBLE, MPI_ANY_SOURCE,
                      MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      int row = status.MPI_TAG;

      int sender = status.MPI_SOURCE;

      memcpy(matrix_c[row], answer, MATRIX_SIZE * sizeof(double));

      if (row_a_counter < MATRIX_SIZE) {

        ierr = MPI_Send(matrix_a[row_a_counter], MATRIX_SIZE, MPI_DOUBLE,
                        sender, row_a_counter + 1, MPI_COMM_WORLD);

        row_a_counter++;

      }

      else {

        // send message with tag 0 so that all the processes can stop
        // calculating

        ierr = MPI_Send(&matrix_c[0], MATRIX_SIZE, MPI_DOUBLE, sender, 0,
                        MPI_COMM_WORLD);
      }
    }

    /*printf("Solution: \n");

    printMatrix(matrix_c);*/

    printf("Solution lower right corner: %.2f\n",
           matrix_c[MATRIX_SIZE - 1][MATRIX_SIZE - 1]);

    free(matrix_a);

    free(matrix_c);

  }

  else {

    buffer = (double *)malloc(MATRIX_SIZE * sizeof(double));

    for (int i = 0; i < MATRIX_SIZE; i++) {

      ierr = MPI_Bcast(matrix_b[i], MATRIX_SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    int tag = 1;

    while (tag != 0) {

      ierr = MPI_Recv(buffer, MATRIX_SIZE, MPI_DOUBLE, 0, MPI_ANY_TAG,
                      MPI_COMM_WORLD, &status);

      tag = status.MPI_TAG;

      // printf("tag: %d\n", tag);

      if (tag != 0) {

        int row = tag - 1;

        multiply_matrices(buffer, answer, matrix_b);

        // send solution

        ierr =
            MPI_Send(answer, MATRIX_SIZE, MPI_DOUBLE, 0, row, MPI_COMM_WORLD);
      }
    }

    // ierr = MPI_Barrier(MPI_COMM_WORLD);

    free(buffer);
  }

  free(matrix_b);

  free(answer);

  // ierr = MPI_Barrier(MPI_COMM_WORLD);

  ierr = MPI_Finalize();
}
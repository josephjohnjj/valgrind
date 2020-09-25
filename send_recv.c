#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  
  MPI_Barrier(MPI_COMM_WORLD);
  double start = MPI_Wtime();

  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int *number = (int*) malloc(sizeof(int));
  if (world_rank == 0) {
    // If we are rank 0, set the number to -1 and send it to process 1
    number[0] = 100;
    number[1] = 200;
    MPI_Send(
      /* data         = */ &number, 
      /* count        = */ 2, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ 1, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD);
  } else if (world_rank == 1) {
      MPI_Recv(
      /* data         = */ &number, 
      /* count        = */ 2, 
      /* datatype     = */ MPI_INT, 
      /* source       = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* status       = */ MPI_STATUS_IGNORE);
    printf("Process 1 received numbers %d, %d from process 0\n", number[0], number[1]);
  }

  double end = MPI_Wtime();
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Finalize();

  
  printf("\n Time (Proc %d) = %f\n", world_rank, end-start);
}

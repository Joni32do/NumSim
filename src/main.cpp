#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <array>


#include "parallel/partitioning.h"
#include "parallel/communicator.h"
#include "computation/computation_parallel.h"
#include "test/test_parallel.h"


int main(int argc, char** argv) {

  MPI_Init(NULL, NULL);

  int own_rank_number;
  MPI_Comm_rank(MPI_COMM_WORLD, &own_rank_number);

  double start_time = MPI_Wtime();

  ComputationParallel computation;
  computation.initializeParallel(argc, argv);
  computation.runSimulationParallel();

  double end_time = MPI_Wtime();

  if(own_rank_number == 0){
    std::cout << "Run time: " << end_time-start_time << std::endl;
  }

  
  MPI_Finalize();
}







/*

// Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int token;
  // Receive from the lower process and send to the higher process. Take care
  // of the special case when you are the first process to prevent deadlock.
  if (world_rank != 0) {
    MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received token %d from process %d\n", world_rank, token,
           world_rank - 1);
  } else {
    // Set the token's value if you are process 0
    token = -1;
  }
  MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0,
           MPI_COMM_WORLD);
  // Now process 0 can receive from the last process. This makes sure that at
  // least one MPI_Send is initialized before all MPI_Recvs (again, to prevent
  // deadlock)
  if (world_rank == 0) {
    MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received token %d from process %d\n", world_rank, token,
           world_size - 1);
  }



*/

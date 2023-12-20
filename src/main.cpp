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


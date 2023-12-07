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


  ComputationParallel computation;
  computation.initializeParallel(argc, argv);
  computation.runSimulationParallel();

  
  MPI_Finalize();
}

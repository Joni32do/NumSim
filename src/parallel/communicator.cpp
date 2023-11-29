#include "communicator.h"


// Wrapper for MPI
Communicator::Communicator(){

    MPI_Comm_rank(MPI_COMM_WORLD, &ownRankNo_);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks_);
}


int Communicator::ownRankNo(){
    return ownRankNo_;
}


int Communicator::nRanks(){
    return nRanks_;
}

double Communicator::getGlobalMin(double local_value){
    double global_value;
    MPI_Allreduce(&local_value, &global_value, 1,  MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    return global_value;
}





// #ifndef NDEBUG
//         // profiling start
//     double START_TIME = MPI_Wtime();


//     // profiling sum
//     partitioning_->DURATION_COMMUNICATION_REDUCE += MPI_Wtime() - START_TIME;
// #endif
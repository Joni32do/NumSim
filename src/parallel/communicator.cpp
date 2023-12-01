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


std::vector<double> Communicator::receiveFrom(int rank, int message_size, int tag=0){
    std::vector<double> received_data;
    MPI_Recv(&received_data, message_size, MPI_DOUBLE, rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return received_data;
}

void Communicator::sendTo(int rank, std::vector<double> buffer, int tag=0){
    MPI_Send(buffer.data(), buffer.size(), MPI_DOUBLE, rank, tag, MPI_COMM_WORLD);
}





// #ifndef NDEBUG
//         // profiling start
//     double START_TIME = MPI_Wtime();


//     // profiling sum
//     partitioning_->DURATION_COMMUNICATION_REDUCE += MPI_Wtime() - START_TIME;
// #endif
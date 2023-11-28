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


std::array<int,2> Communicator::nCellsLocal(std::array<int,2> nCellsGlobal){
    return {nCellsGlobal[0]/nProcesses_[0], nCellsGlobal[1]/nProcesses_[1]};
}


void Communicator::findOptimumProcessAlignment(std::array<int,2> nCellsGlobal){

    int costMinimal = 99999;
    int cost = 0;
    int p_xMin = 0;
    int p_yMin = 0;
    int p_x;
    int p_y;

    for (int i = 1; i <= nRanks_; i++)
    {
        if (nRanks_ % i == 0)
        {
            p_x = i;
            p_y = nRanks_ / i;
            cost = nCellsGlobal[0] * (p_x - 1) + nCellsGlobal[1] * (p_y - 1);
            if (cost < costMinimal){
                costMinimal = cost;
                p_xMin = p_x;
                p_yMin = p_y;
            }
        }
    }
    nProcesses_[0] = p_xMin;
    nProcesses_[1] = p_yMin;

}

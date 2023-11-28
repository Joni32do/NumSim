#include "partitioning.h"

Partitioning::Partitioning(std::array<int,2> nCellsGlobal,
                           std::shared_ptr<Communicator> communicator):
            nCellsGlobal_(nCellsGlobal),
            nCellsLocal_(communicator->nCellsLocal(nCellsGlobal)),
            ownRankNo_(communicator->ownRankNo()),
            nRanks_(communicator->nRanks())
{

    // MPI_Comm_rank(MPI_COMM_WORLD, &ownRankNo_);
    // MPI_Comm_size(MPI_COMM_WORLD, &nRanks_);
    // nCellsGlobal_ = nCellsGlobal;

    // calculateProcessesPerDimension();
    // ownProcess_ = {ownRankNo_%nProcesses_[0], ownRankNo/nProcesses_[1]};





    // std::cout << "This should be the n processes" << std::endl;
    // std::cout << nRanks_ << std::endl;
    // std::cout << ownProcess_[0] << "und" << ownProcess_[0] << std::endl;
 

}

// std::array<int,2> Partitioning::nCellsLocal() const{
//     nProcesses_[0];
//     return nCellsLocal;
// }


// std::array<int,2> Partitioning::nCellsGlobal() const{
//     return nCellsGlobal;
// }

// TODO: Refactor this to `process_manager`

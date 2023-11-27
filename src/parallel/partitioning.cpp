#include "partitioning.h"

Partitioning::Partitioning(std::array<int,2> nCellsGlobal){
    MPI_Comm_rank(MPI_COMM_WORLD, &ownRankNo);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    std::cout << "This should be the n processes" << std::endl;
    std::cout << nRanks << std::endl;

}

// std::array<int,2> nCellsLocal(){
//     return 
// }
#include "partitioning.h"

Partitioning::Partitioning(std::array<int,2> nCellsGlobal){
    MPI_Comm_rank(MPI_COMM_WORLD, &ownRankNo);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    std::cout << "This should be the n processes" << std::endl;
    std::cout << nRanks << std::endl;

    

}

std::array<int,2> Partitioning::nCellsLocal() const{
    return nCellsLocal;
}


std::array<int,2> Partitioning::nCellsGlobal() const{
    return nCellsGlobal;
}

// TODO: Refactor this to `process_manager`
void Partitioning::get_domain(){

    int costMinimal = 99999;
    int cost = 0;
    int p_xMin = 0;
    int p_yMin = 0;

    for (int i = 1; i <= nRanks; i++)
    {
        if (nRanks % i == 0)
        {
            int p_x = i;
            int p_y = nRanks / i;
            cost = nCellsGlobal[0] * (p_x - 1) + nCellsGlobal[1] * (p_y - 1);
            if (cost < costMinimal){
                costMinimal = cost;
                p_xMin = p_x;
                p_yMin = p_y;
            }
        }
    }
}

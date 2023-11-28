#include "partitioning.h"

Partitioning::Partitioning(std::array<int,2> nCellsGlobal,
                           std::shared_ptr<Communicator> communicator):
            nProcesses_(findOptimumProcessAlignment(nCellsGlobal)),
            nCellsGlobal_(nCellsGlobal),
            nCellsLocal_(calculateNCellsLocal(nCellsGlobal)),
            ownRankNo_(communicator->ownRankNo()),
            nRanks_(communicator->nRanks()),
            communicator_(communicator)
{
    std::cout << ownRankNo_ << std::endl;
    std::cout << nRanks_ << std::endl;
    std::cout << nProcesses_[0] << " and yProcesses " << nProcesses_[1] << std::endl;
}


std::array<int,2> Partitioning::nCellsLocal() const{
    return nCellsLocal_;
}


std::array<int,2> Partitioning::nCellsGlobal() const{
    return nCellsGlobal_;
}

int Partitioning::ownRankNo() const{
    return ownRankNo_;
}

int Partitioning::nRanks() const{
    return nRanks_;
}

bool Partitioning::ownPartitionContainsBottomBoundary() const{
    if (ownRankNo_ < nProcesses_[0]){
        return true;
    } else {
        return false;
    }
}

bool Partitioning::ownPartitionContainsTopBoundary() const{
    if (ownRankNo_ >= nProcesses_[0] * (nProcesses_[1] - 1)){
        return true;
    } else {
        return false;
    }
}

bool Partitioning::ownPartitionContainsLeftBoundary() const{
    if (ownRankNo_%nProcesses_[0] == 0){
        return true;
    } else {
        return false;
    }
}

bool Partitioning::ownPartitionContainsRightBoundary() const{
    if (ownRankNo_%nProcesses_[0] == nProcesses_[0] - 1){
        return true;
    } else {
        return false;
    }
}

int Partitioning::leftNeighbourRankNo() const{
    assert(!ownPartitionContainsLeftBoundary());
    return ownRankNo_ - 1;
}


int Partitioning::rightNeighbourRankNo() const{
    assert(!ownPartitionContainsRightBoundary());
    return ownRankNo_ + 1;
    
}

int Partitioning::topNeighbourRankNo() const{
    assert(!ownPartitionContainsTopBoundary());
    return ownRankNo_ + nProcesses_[0];
}

int Partitioning::bottomNeighbourRankNo() const{
    assert(!ownPartitionContainsBottomBoundary());
    return ownRankNo_ - nProcesses_[0];
}

std::array<int,2> Partitioning::nodeOffset() const{
    return {nCellsGlobal_[0] - nCellsLocal_[0], nCellsGlobal_[1] - nCellsLocal_[1]};
}



std::array<int,2> Partitioning::findOptimumProcessAlignment(std::array<int,2> nCellsGlobal){
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
    return {p_xMin, p_yMin};
}



std::array<int,2> Partitioning::calculateNCellsLocal(std::array<int,2> nCellsGlobal){
    // TODO: more advanced implementation
    return {nCellsGlobal[0]/nProcesses_[0], nCellsGlobal[1]/nProcesses_[1]};
}


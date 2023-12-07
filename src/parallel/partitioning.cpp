#include "partitioning.h"

Partitioning::Partitioning(std::array<int, 2> nCellsGlobal,
                           std::shared_ptr<Communicator> communicator) : nCellsGlobal_(nCellsGlobal),
                                                                         communicator_(communicator),
                                                                         nRanks_(communicator->nRanks()),
                                                                         ownRankNo_(communicator->ownRankNo())
{
    nProcesses_ = findOptimumProcessAlignment();
    ownProcess_ = {ownRankNo_ % nProcesses_[0], ownRankNo_ / nProcesses_[0]};
    nCellsLocal_ = calculateNCellsLocal();
    calcNodeOffset();
    calcLowerLeftIsRed();
}

std::array<int, 2> Partitioning::nCellsLocal() const
{
    return nCellsLocal_;
}

std::array<int, 2> Partitioning::nCellsGlobal() const
{
    return nCellsGlobal_;
}

int Partitioning::ownRankNo() const
{
    return ownRankNo_;
}

int Partitioning::nRanks() const
{
    return nRanks_;
}

bool Partitioning::ownPartitionContainsBottomBoundary() const
{
    if (ownRankNo_ < nProcesses_[0])
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Partitioning::ownPartitionContainsTopBoundary() const
{
    if (ownRankNo_ >= nProcesses_[0] * (nProcesses_[1] - 1))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Partitioning::ownPartitionContainsLeftBoundary() const
{
    if (ownRankNo_ % nProcesses_[0] == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Partitioning::ownPartitionContainsRightBoundary() const
{
    if (ownRankNo_ % nProcesses_[0] == nProcesses_[0] - 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Partitioning::leftNeighbourRankNo() const
{
    assert(!ownPartitionContainsLeftBoundary());
    return ownRankNo_ - 1;
}

int Partitioning::rightNeighbourRankNo() const
{
    assert(!ownPartitionContainsRightBoundary());
    return ownRankNo_ + 1;
}

int Partitioning::topNeighbourRankNo() const
{
    assert(!ownPartitionContainsTopBoundary());
    return ownRankNo_ + nProcesses_[0];
}

int Partitioning::bottomNeighbourRankNo() const
{
    assert(!ownPartitionContainsBottomBoundary());
    return ownRankNo_ - nProcesses_[0];
}

void Partitioning::calcNodeOffset()
{
    int x_modulo = remainderLocalCells_[0];
    int y_modulo = remainderLocalCells_[1];

    int x_offset = 0;
    int y_offset = 0;

    int standard_n_cells_x = nCellsGlobal_[0] / nProcesses_[0];
    int standard_n_cells_y = nCellsGlobal_[1] / nProcesses_[1];

    for (int i = 0; i < ownProcess_[0]; i++)
    {
        if (x_modulo == 0)
        {
            x_offset += standard_n_cells_x;
        }
        else
        {
            x_offset += standard_n_cells_x + 1;
            x_modulo -= 1;
        }
    }

    for (int j = 0; j < ownProcess_[1]; j++)
    {
        if (y_modulo == 0)
        {
            y_offset += standard_n_cells_y;
        }
        else
        {
            y_offset += standard_n_cells_y + 1;
            y_modulo -= 1;
        }
    }

    nodeOffset_ = {x_offset, y_offset};
}

std::array<int, 2> Partitioning::nodeOffset() const
{

    return nodeOffset_;
}

void Partitioning::calcLowerLeftIsRed()
{
    lowerLeftIsRed_ = (nodeOffset_[0] + nodeOffset_[1]) % 2 == 0;
}

bool Partitioning::lowerLeftIsRed()
{
    return lowerLeftIsRed_;
}


std::array<int, 2> Partitioning::findOptimumProcessAlignment()
{
    int costMinimal = 9999999;
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
            cost = nCellsGlobal_[0] * (p_x - 1) + nCellsGlobal_[1] * (p_y - 1);
            if (cost < costMinimal)
            {
                costMinimal = cost;
                p_xMin = p_x;
                p_yMin = p_y;
            }
        }
    }
    return {p_xMin, p_yMin};
}

std::array<int, 2> Partitioning::calculateNCellsLocal()
{
    std::array<int, 2> nCellsLocal;

    for (int i = 0; i < 2; i++)
    {
        remainderLocalCells_[i] = nCellsGlobal_[i] % nProcesses_[i];
        if (ownProcess_[i] < remainderLocalCells_[i])
        {
            nCellsLocal[i] = nCellsGlobal_[i] / nProcesses_[i] + 1;
        }
        else
        {
            nCellsLocal[i] = nCellsGlobal_[i] / nProcesses_[i];
        }
    }
    return nCellsLocal;
}


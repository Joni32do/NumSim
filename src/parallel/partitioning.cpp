#include "partitioning.h"

Partitioning::Partitioning(std::array<int,2> nCellsGlobal,
                           std::shared_ptr<Communicator> communicator):
            nCellsGlobal_(nCellsGlobal),
            communicator_(communicator),
            nRanks_(communicator->nRanks()),
            ownRankNo_(communicator->ownRankNo())
{
    nProcesses_ = findOptimumProcessAlignment();
    ownProcess_ = {ownRankNo_%nProcesses_[0], ownRankNo_/nProcesses_[0]};
    nCellsLocal_ = calculateNCellsLocal();
    calcNodeOffset();
    calcLowerLeftIsRed();

    // Contains Boundaries
    containsBottomBoundary_ = (ownRankNo_ < nProcesses_[0]);
    containsTopBoundary_ = (ownRankNo_ >= nProcesses_[0] * (nProcesses_[1] - 1));
    containsLeftBoundary_ = (ownRankNo_%nProcesses_[0] == 0);
    containsRightBoundary_ = (ownRankNo_%nProcesses_[0] == nProcesses_[0] - 1);
    
    // Calculate Rank Numbers (also invalid once)
    bottomNeighbourRankNo_ = ownRankNo_ - nProcesses_[0];
    topNeighbourRankNo_ = ownRankNo_ + nProcesses_[0];
    leftNeighbourRankNo_ = ownRankNo_ - 1;
    rightNeighbourRankNo_ = ownRankNo_ + 1;
    
#ifndef NDEBUG
    printDebugInformation();
#endif
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
    return containsBottomBoundary_;
}

bool Partitioning::ownPartitionContainsTopBoundary() const{
    return containsTopBoundary_;
}

bool Partitioning::ownPartitionContainsLeftBoundary() const{
    return containsLeftBoundary_;
}

bool Partitioning::ownPartitionContainsRightBoundary() const{
    return containsRightBoundary_;
}

int Partitioning::bottomNeighbourRankNo() const{
    assert(!ownPartitionContainsBottomBoundary());
    return bottomNeighbourRankNo_;
}

int Partitioning::topNeighbourRankNo() const{
    assert(!ownPartitionContainsTopBoundary());
    return topNeighbourRankNo_;
}

int Partitioning::leftNeighbourRankNo() const{
    assert(!ownPartitionContainsLeftBoundary());
    return leftNeighbourRankNo_;
}

int Partitioning::rightNeighbourRankNo() const{
    assert(!ownPartitionContainsRightBoundary());
    return rightNeighbourRankNo_;
    
}

void Partitioning::calcNodeOffset(){
    int x_modulo = remainderLocalCells_[0];
    int y_modulo = remainderLocalCells_[1];

    int x_offset = 0;
    int y_offset = 0; 

    int standard_n_cells_x = nCellsGlobal_[0]/nProcesses_[0];
    int standard_n_cells_y = nCellsGlobal_[1]/nProcesses_[1];


    for(int i =0; i< ownProcess_[0]; i++){
        if(x_modulo == 0){
            x_offset += standard_n_cells_x;
        }
        else{
            x_offset += standard_n_cells_x+1;
            x_modulo -= 1;
        }
    }
    
    for(int j =0; j< ownProcess_[1]; j++){
        if(y_modulo == 0){
            y_offset += standard_n_cells_y;
        }
        else{
            y_offset += standard_n_cells_y+1;
            y_modulo -= 1;
        }
    }

    nodeOffset_ = {x_offset, y_offset};
}

std::array<int,2> Partitioning::nodeOffset() const{
    

    return nodeOffset_;
}

// TODO: remove only for debug
std::array<int,2> Partitioning::nProcesses(){
    return nProcesses_;
}

void Partitioning::calcLowerLeftIsRed(){
    lowerLeftIsRed_ = (nodeOffset_[0] + nodeOffset_[1])%2 == 0;
}

bool Partitioning::lowerLeftIsRed(){
    return lowerLeftIsRed_;
}

bool Partitioning::sendsFirstUpDown(){
    return (ownProcess_[1]%2) == 0;
}

bool Partitioning::sendsFirstLeftRight(){
    return (ownProcess_[0]%2) == 0;
}



std::array<int,2> Partitioning::findOptimumProcessAlignment(){
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
            if (cost < costMinimal){
                costMinimal = cost;
                p_xMin = p_x;
                p_yMin = p_y;
            }
        }
    }
    return {p_xMin, p_yMin};
}



std::array<int,2> Partitioning::calculateNCellsLocal(){
    // TODO: more advanced implementation
    std::array<int,2> nCellsLocal;
    

    for (int i = 0; i < 2; i++){
        // std::cout << "fsdProcess: x:" << ownProcess_[0] << " von " << nProcesses_[0]
        //       << " y: " << ownProcess_[1] << " von " << nProcesses_[1] << std::endl;
        remainderLocalCells_[i] = nCellsGlobal_[i]%nProcesses_[i];
        // std::cout << "Write" << i << " c " << remainderLocalCells_[i] << std::endl;
        if (ownProcess_[i] < remainderLocalCells_[i]){
            nCellsLocal[i] = nCellsGlobal_[i]/nProcesses_[i] + 1;
        } else {
            nCellsLocal[i] = nCellsGlobal_[i]/nProcesses_[i];
        }
    }
    return nCellsLocal;
}


void Partitioning::printDebugInformation(){
    std::array<int,2> node_offset_ = nodeOffset();

    Printer printer_(ownRankNo_);
    std::string out_string_1 = "Rank: " + std::to_string(ownRankNo_ ) + " von " + std::to_string(nRanks_);
    std::string out_string_2 = "Nodes offset x: " + std::to_string(node_offset_[0]) + " " +"Nodes offset y: " + std::to_string(node_offset_[1]);
    printer_.add_new_parameter_to_print(out_string_1);
    printer_.add_new_parameter_to_print(out_string_2);

    printer_.save_values_to_file();
    
    //std::cout << "Print Debug Info" << std::endl
    //std::cout << "Rank: " << ownRankNo_ << " von " << nRanks_ << std::endl
    //<< "n_cells_x: " << nCellsGlobal_[0] << ", n_cells_y: " << nCellsGlobal_[1] << std::endl
    //<< "Processes x-direction: " << nProcesses_[0] << ", Processes y-direction: " << nProcesses_[1] << std::endl
   // << "Cells Local: x:" << nCellsLocal_[0] << " y:" << nCellsLocal_[1] << std::endl
    //<< "node_offset x " << node_offset_[0] << " node_offset y " << node_offset_[1] << "\n";
}
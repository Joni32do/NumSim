#pragma once

#include <mpi.h>

#include <memory>
#include <array>
#include <iostream>


class Communicator{

    public:
        Communicator();

        int ownRankNo();

        int nRanks();

        void findOptimumProcessAlignment(std::array<int,2> nCellsGlobal);

        std::array<int,2> nCellsLocal(std::array<int,2> nCellsGlobal);

              



    private:
        // std::shared_ptr<Partitioning> partitioning_ = 
        //                 std::make_shared<Partitioning>(nCellsGlobal);
        int ownRankNo_;
        int nRanks_;
        std::array<int,2> nProcesses_;

};
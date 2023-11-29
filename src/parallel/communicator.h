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
        double getGlobalMin(double local_value);


    private:
        int ownRankNo_;
        int nRanks_;
        //std::array<int,2> nProcesses_;

};



/*

    if (communicator_->isMain()){


        comminicator->sendToSubs();
    } else {
        nProcesses_ = communicator_->getFromMain();
    }

*/

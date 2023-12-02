#pragma once

#include <mpi.h>

#include <memory>
#include <array>
#include <vector>
#include <iostream>
//#include "partitioning.h"


class Communicator{

    public:
        Communicator();

        int ownRankNo();
        int nRanks();
        double getGlobalMin(double local_value);


        void sendTo(int rank, std::vector<double> buffer, int tag=0);
        std::vector<double> receiveFrom(int rank, int message_size, int tag=0);


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

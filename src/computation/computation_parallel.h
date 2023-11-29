#pragma once

#include <memory>
#include <cmath>

#include "computation.h"
#include "../parallel/communicator.h"
#include "../parallel/partitioning.h"

class ComputationParallel: Computation{

    public:
        void initializeParallel(int argc, char *argv[]);
        void runSimulationParallel();

        void computeTimeStepWidthParallel(double currentTime);


    private:
        std::shared_ptr<Communicator> communicator_;
        std::shared_ptr<Partitioning> partitioning_;

};

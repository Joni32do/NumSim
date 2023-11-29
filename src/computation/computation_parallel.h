#pragma once

#include <memory>

#include "computation.h"
#include "../parallel/communicator.h"

class ComputationParallel: Computation{

    public:
        void runSimulation();


    private:
        std::shared_ptr<Communicator> communicator;


};

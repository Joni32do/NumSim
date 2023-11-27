#pragma once

#include "computation.h"

class ComputationParallel: Computation{

    public:
        void runSimulation();


    private:
        int i_rank;
        int n_rank;


};

#pragma once

#include <memory>
#include <cmath>

#include "computation/computation.h"
#include "parallel/communicator.h"
#include "parallel/partitioning.h"

#include "debugging/debug_printer.h"
#include "output_writer/output_writer_text_parallel.h"
#include "solver/red_black.h"

class ComputationParallel: Computation{

    public:
        void initializeParallel(int argc, char *argv[]);
        void runSimulationParallel();

        void applyBoundaryValuesParallel();
        void computeTimeStepWidthParallel(double currentTime);
        void computePreliminaryVelocitiesParallel();
        void computeRightHandSideParallel();
        void computeVelocitiesParallel();

    private:
        std::shared_ptr<Communicator> communicator_;
        std::shared_ptr<Partitioning> partitioning_;
        // std::unique_ptr<OutputWriterParaviewParallel> outputWriterParaviewParallel_;

};

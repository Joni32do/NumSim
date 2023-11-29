#include "computation_parallel.h"

void ComputationParallel::initializeParallel(int argc, char *argv[]){

    std::string filename = argv[1];

    // load settings from file
    settings_.loadFromFile(filename);
#ifndef NDEBUG
    settings_.printSettings();
#endif

    communicator_ = std::make_shared<Communicator>();
    partitioning_ = std::make_shared<Partitioning>(settings_.nCells,
                                            communicator_);
                                            
    std::array<double, 2> meshWidth_ = {settings_.physicalSize[0] / settings_.nCells[0],
                                        settings_.physicalSize[1] / settings_.nCells[1]};

    if (settings_.useDonorCell)
    {
        discretization_ = std::make_shared<DonorCell>(partitioning_->nCellsLocal(), meshWidth_, settings_.alpha);
    }
    else
    {
        discretization_ = std::make_shared<CentralDifferences>(partitioning_->nCellsLocal(), meshWidth_);
    }

    if (settings_.pressureSolver == "SOR")
    {
        pressureSolver_ = std::make_unique<SOR>(discretization_,
                                                settings_.epsilon,
                                                settings_.maximumNumberOfIterations,
                                                settings_.omega);
    }
    else
    {
        pressureSolver_ = std::make_unique<GaussSeidel>(discretization_,
                                                        settings_.epsilon,
                                                        settings_.maximumNumberOfIterations);
    }

    outputWriterParaview_ = std::make_unique<OutputWriterParaview>(discretization_);
    outputWriterText_ = std::make_unique<OutputWriterText>(discretization_);


}

void ComputationParallel::runSimulationParallel(){
    double currentTime = 0;
    computeTimeStepWidthParallel(currentTime);
    //std::cout << "Time Step" << dt_ << std::endl;
    }

void ComputationParallel::computeTimeStepWidthParallel(double currentTime){
    computeTimeStepWidth(currentTime);
    dt_ = communicator_->getGlobalMin(dt_);

    // Print every whole second
    double nextWholeSecond = std::ceil(currentTime);
    if (currentTime + dt_ > nextWholeSecond)
    {
        dt_ = nextWholeSecond - currentTime;
    };
}

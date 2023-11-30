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

    if (settings_.pressureSolver == "RedBlack"){
        // TODO:
        // pressureSolver_ = 
    }



    // outputWriterParaviewParallel_ = std::make_unique<OutputWriterParaviewParallel>(
    //                                             discretization_, partitioning_);


}

void ComputationParallel::runSimulationParallel(){
    double currentTime = 0;

    // TODO: remove prints
    Printer printer_(communicator_->ownRankNo());

    do
    {
        applyBoundaryValuesParallel();
        // computeTimeStepWidthParallel(currentTime);
        // currentTime += dt_;
        currentTime += 5;


        

        // DEBUGGING 
        std::string str = "Rank: " + std::to_string(communicator_->ownRankNo()) 
                        + " PartX" + std::to_string(partitioning_->nProcesses()[0]) 
                        + " PartY" + std::to_string(partitioning_->nProcesses()[1])
                        + " Zeit " + std::to_string(currentTime) 
                        + " dt"    + std::to_string(dt_);
        printer_.add_new_parameter_to_print(str);

        std::string lineOfArray2D = "Array " + std::to_string(discretization_->v(0, 1))
                                     + " | " + std::to_string(discretization_->v(1, 1))
                                     + " | " + std::to_string(discretization_->v(2, 1))
                                     + " | " + std::to_string(discretization_->v(3, 1))
                                     + " | " + std::to_string(discretization_->v(4, 1))
                                     + " | " + std::to_string(discretization_->v(5, 1));

        printer_.add_new_parameter_to_print(lineOfArray2D);
        // DEBUGGING END



        // outputWriterParaviewParallel_->writeFile(currentTime);

    } while (currentTime < settings_.endTime);
    
    // TODO: remove prints
    printer_.save_values_to_file();
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


void ComputationParallel::applyBoundaryValuesParallel(){

    // TODO: Think about Corners of the Domain

//     if (partitioning_->ownPartitionContainsBottomBoundary()){
//         // u
//         for (int i = discretization_->uIBegin(); i < discretization_->uIEnd(); i++){
//             discretization_->u(i, discretization_->uJBegin()) = 
//                                     2 * settings_.dirichletBcBottom[0] 
//                                     - discretization_->u(i, discretization_->uJBegin()+1);
//         }
//         // v
//         for (int i = discretization_->vIBegin(); i < discretization_->vIEnd(); i++){
//             discretization_->v(i, discretization_->vJBegin()) = 
//                                     settings_.dirichletBcBottom[1];
//         }
//         // f
//         for (int i = discretization_->fIBegin(); i < discretization_->fIEnd(); i++){
//             discretization_->f(i, discretization_->fJBegin()) =
//                                     discretization_->u(i, discretization_->fJBegin());
//         }
//         // g
//         for (int i = discretization_->gIBegin(); i < discretization_->gIEnd(); i++){
//             discretization_->g(i, discretization_->gJBegin()) =
//                                     discretization_->v(i, discretization_->gJBegin());
//         }
        
//     }

//     if (partitioning_->ownPartitionContainsTopBoundary()){
//         // u
//         for (int i = discretization_->uIBegin(); i < discretization_->uIEnd(); i++){
//             discretization_->u(i, discretization_->uJEnd() - 1) = 
//                                     2 * settings_.dirichletBcTop[0] 
//                                     - discretization_->u(i, discretization_->uJEnd() - 2);
//         }
//         // v
//         for (int i = discretization_->vIBegin(); i < discretization_->vIEnd(); i++){
//             discretization_->v(i, discretization_->vJEnd() - 1) = 
//                                     settings_.dirichletBcTop[1];
//         }
//         // f
//         for (int i = discretization_->fIBegin(); i < discretization_->fIEnd(); i++){
//             discretization_->f(i, discretization_->fJEnd() - 1) =
//                                     discretization_->u(i, discretization_->fJEnd() - 1);
//         }
//         // g
//         for (int i = discretization_->gIBegin(); i < discretization_->gIEnd(); i++){
//             discretization_->g(i, discretization_->gJEnd() - 1) =
//                                     discretization_->v(i, discretization_->gJEnd() - 1);
//         }
        
//     }

//    if (partitioning_->ownPartitionContainsLeftBoundary()){
//         // u
//         for (int j = discretization_->uJBegin(); discretization_->uJEnd(); j++){
//             discretization_->u(discretization_->uIBegin(), j) =
//                                     settings_.dirichletBcLeft[0];
//         }
//         // v
//         for (int j = discretization_->vJBegin(); discretization_->vJEnd(); j++){
//             discretization_->v(discretization_->vIBegin(), j) =
//                                     2 * settings_.dirichletBcLeft[1]
//                                     - discretization_->v(discretization_->vIBegin() + 1, j);
//         }
//         // f
//         for (int j = discretization_->fJBegin(); discretization_->fJEnd(); j++){
//             discretization_->f(discretization_->fIBegin(), j) =
//                                     discretization_->u(discretization_->fIBegin(), j);
//         }
//         // g TODO: Check if this or the interior -> I think we can assign it, but its never used
//         for (int j = discretization_->gJBegin(); discretization_->gJEnd(); j++){
//             discretization_->g(discretization_->gIBegin(), j) =
//                                     discretization_->v(discretization_->gIBegin(), j);
//         }
//    }

//     if (partitioning_->ownPartitionContainsRightBoundary()){
//         // u
//         for (int j = discretization_->uJBegin(); discretization_->uJEnd(); j++){
//             discretization_->u(discretization_->uIEnd() - 1, j) =
//                                     settings_.dirichletBcRight[0];
//         }
//         // v
//         for (int j = discretization_->vJBegin(); discretization_->vJEnd(); j++){
//             discretization_->v(discretization_->vIEnd() - 1, j) =
//                                     2 * settings_.dirichletBcRight[1]
//                                     - discretization_->v(discretization_->vIEnd() - 2, j);
//         }
//         // f
//         for (int j = discretization_->fJBegin(); discretization_->fJEnd(); j++){
//             discretization_->f(discretization_->fIEnd() - 1, j) =
//                                     discretization_->u(discretization_->fIEnd() - 1, j);
//         }
//         // g TODO: Check if this or the interior -> I think we can assign it, but its never used
//         for (int j = discretization_->gJBegin(); discretization_->gJEnd(); j++){
//             discretization_->g(discretization_->gIEnd() - 1, j) =
//                                     discretization_->v(discretization_->gIEnd() - 1, j);
//         }

//    }
}

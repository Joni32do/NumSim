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
    OutputWriterTextParallel out = OutputWriterTextParallel(discretization_, *partitioning_); 
    do
    {   
        applyBoundaryValuesParallel();
        computePreliminaryVelocities();
        //computeTimeStepWidthParallel(currentTime);
        // currentTime += dt_;
        currentTime += 5;



        // DEBUGGING 


        std::string str = "Rank: " + std::to_string(communicator_->ownRankNo()) 
                                     + " PartX" + std::to_string(partitioning_->nProcesses()[0]) 
                                     + " PartY" + std::to_string(partitioning_->nProcesses()[1])
                                     + " Zeit " + std::to_string(currentTime) 
                                     + " dt"    + std::to_string(dt_);
        printer_.add_new_parameter_to_print(str);

        std::string uLineOfArray2D = "Arr u "+ std::to_string(discretization_->u(0, 0))
                                     + " | " + std::to_string(discretization_->u(1, 0))
                                     + " | " + std::to_string(discretization_->u(2, 0))
                                     + " | " + std::to_string(discretization_->u(3, 0))
                                     + " | " + std::to_string(discretization_->u(4, 0))
                                     + " | " + std::to_string(discretization_->u(5, 0));
        printer_.add_new_parameter_to_print(uLineOfArray2D);

        std::string vLineOfArray2D = "Arr v "+ std::to_string(discretization_->v(0, 1))
                                      + " | " + std::to_string(discretization_->v(1, 1))
                                      + " | " + std::to_string(discretization_->v(2, 1))
                                      + " | " + std::to_string(discretization_->v(3, 1))
                                      + " | " + std::to_string(discretization_->v(4, 1))
                                      + " | " + std::to_string(discretization_->v(5, 1));
        printer_.add_new_parameter_to_print(vLineOfArray2D);

        int u_last = discretization_->uJEnd()-1;
        std::string uLineOfArray2DTop = "Arr u "+ std::to_string(discretization_->u(0, u_last))
                                        + " | " + std::to_string(discretization_->u(1, u_last))
                                        + " | " + std::to_string(discretization_->u(2, u_last))
                                        + " | " + std::to_string(discretization_->u(3, u_last))
                                        + " | " + std::to_string(discretization_->u(4, u_last))
                                        + " | " + std::to_string(discretization_->u(5, u_last));
        printer_.add_new_parameter_to_print(uLineOfArray2DTop);
        int v_last = discretization_->vJEnd()-1;
        std::string vLineOfArray2DTop = "Arr v "+ std::to_string(discretization_->v(0, v_last))
                                        + " | " + std::to_string(discretization_->v(1, v_last))
                                        + " | " + std::to_string(discretization_->v(2, v_last))
                                        + " | " + std::to_string(discretization_->v(3, v_last))
                                        + " | " + std::to_string(discretization_->v(4, v_last))
                                        + " | " + std::to_string(discretization_->v(5, v_last));
        printer_.add_new_parameter_to_print(vLineOfArray2DTop);


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

    //TODO: Think about Corners of the Domain

    if (partitioning_->ownPartitionContainsBottomBoundary()){
        // u
        for (int i = discretization_->uIBegin(); i < discretization_->uIEnd(); i++){
            discretization_->u(i, discretization_->uJBegin()) = 
                                    2 * settings_.dirichletBcBottom[0] 
                                    - discretization_->u(i, discretization_->uJBegin()+1);
        }
        // v
        for (int i = discretization_->vIBegin(); i < discretization_->vIEnd(); i++){
            discretization_->v(i, discretization_->vJBegin()) = 
                                    settings_.dirichletBcBottom[1];
        }        
    }
    

    if (partitioning_->ownPartitionContainsTopBoundary()){
        // u
        for (int i = discretization_->uIBegin(); i < discretization_->uIEnd(); i++){
            discretization_->u(i, discretization_->uJEnd() - 1) = 
                                    2 * settings_.dirichletBcTop[0] 
                                    - discretization_->u(i, discretization_->uJEnd() - 2);
        }
        // v
        for (int i = discretization_->vIBegin(); i < discretization_->vIEnd(); i++){
            discretization_->v(i, discretization_->vJEnd() - 1) = 
                                    settings_.dirichletBcTop[1];
        }        
    }
    

   if (partitioning_->ownPartitionContainsLeftBoundary()){
        // u
        for (int j = discretization_->uJBegin(); j < discretization_->uJEnd(); j++){
            discretization_->u(discretization_->uIBegin(), j) =
                                    settings_.dirichletBcLeft[0];
        }
        // v
        for (int j = discretization_->vJBegin(); j < discretization_->vJEnd(); j++){
            discretization_->v(discretization_->vIBegin(), j) =
                                    2 * settings_.dirichletBcLeft[1]
                                    - discretization_->v(discretization_->vIBegin() + 1, j);
        }
   }

   

    if (partitioning_->ownPartitionContainsRightBoundary()){
        // u
        for (int j = discretization_->uJBegin(); j < discretization_->uJEnd(); j++){
            discretization_->u(discretization_->uIEnd() - 1, j) =
                                    settings_.dirichletBcRight[0];
        }
        // v
        for (int j = discretization_->vJBegin(); j < discretization_->vJEnd(); j++){
            discretization_->v(discretization_->vIEnd() - 1, j) =
                                    2 * settings_.dirichletBcRight[1]
                                    - discretization_->v(discretization_->vIEnd() - 2, j);
        }
   }
}

void ComputationParallel::computePreliminaryVelocitiesParallel(){

    int f_i_beg = discretization_->fIBegin();
    int f_i_end = discretization_->fIEnd();
    int f_j_beg = discretization_->fJBegin();
    int f_j_end = discretization_->fJEnd();

    int g_i_beg = discretization_->gIBegin();
    int g_i_end = discretization_->gIEnd();
    int g_j_beg = discretization_->gJBegin();
    int g_j_end = discretization_->gJEnd();

    if (partitioning_->ownPartitionContainsLeftBoundary()){
        f_i_beg += 1;
        // f
        for (int j = discretization_->fJBegin(); j < discretization_->fJEnd(); j++){
            discretization_->f(discretization_->fIBegin(), j) =
                                    discretization_->u(discretization_->fIBegin(), j);
        }
    }

    if (partitioning_->ownPartitionContainsRightBoundary()){
        f_i_end -= 1;
        // f
        for (int j = discretization_->fJBegin(); j < discretization_->fJEnd(); j++){
            discretization_->f(discretization_->fIEnd() - 1, j) =
                                    discretization_->u(discretization_->fIEnd() - 1, j);
        }
    }

    if (partitioning_->ownPartitionContainsBottomBoundary()){
        g_j_beg += 1;
        // g
        for (int i = discretization_->gIBegin(); i < discretization_->gIEnd(); i++){
            discretization_->g(i, discretization_->gJBegin()) =
                                    discretization_->v(i, discretization_->gJBegin());
        }
    }

    if (partitioning_->ownPartitionContainsTopBoundary()){
        g_j_end -= 1;
        // g
        for (int i = discretization_->gIBegin(); i < discretization_->gIEnd(); i++){
            discretization_->g(i, discretization_->gJEnd() - 1) =
                                    discretization_->v(i, discretization_->gJEnd() - 1);
        }
    }

    // Interior of F
    for (int i = f_i_beg + 1; i < f_i_end - 1; i++)
    {
        for (int j = f_j_beg; j < f_j_end; j++)
        {
            double diffusion = 1 / settings_.re * (discretization_->computeD2uDx2(i, j) + discretization_->computeD2uDy2(i, j));
            double convection = -discretization_->computeDu2Dx(i, j) - discretization_->computeDuvDy(i, j);
            
            discretization_->f(i, j) = discretization_->u(i, j) + dt_ * (diffusion + convection + settings_.g[0]);
        }
    }

   // Interior of G
    for (int i = g_i_beg; i < g_i_end; i++)
    {
        for (int j = g_j_beg + 1; j < g_j_end - 1; j++)
        {
            double diffusion = 1 / settings_.re * (discretization_->computeD2vDx2(i, j) + discretization_->computeD2vDy2(i, j));
            double convection = -discretization_->computeDv2Dy(i, j) - discretization_->computeDuvDx(i, j);

            discretization_->g(i, j) = discretization_->v(i, j) + dt_ * (diffusion + convection + settings_.g[1]);
        }
    }

   
}

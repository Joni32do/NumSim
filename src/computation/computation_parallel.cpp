#include "computation_parallel.h"

void ComputationParallel::initializeParallel(int argc, char *argv[]){

    std::string filename = argv[1];

    // load settings from file
    settings_.loadFromFile(filename);
// #ifndef NDEBUG
//     settings_.printSettings();
// #endif

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
        pressureSolver_ = std::make_unique<RedBlack>(discretization_,
                                                     settings_.epsilon, 
                                                     settings_.maximumNumberOfIterations,
                                                     communicator_,
                                                     partitioning_);
    }



    outputWriterParaviewParallel_ = std::make_unique<OutputWriterParaviewParallel>(
                                                 discretization_, *partitioning_);


}

void ComputationParallel::runSimulationParallel(){
    double currentTime = 0;

    // TODO: remove prints
    Printer printer_(communicator_->ownRankNo());
    OutputWriterTextParallel out = OutputWriterTextParallel(discretization_, *partitioning_); 
    do
    {   
        applyBoundaryValuesParallel();
        computeTimeStepWidthParallel(currentTime);
        computePreliminaryVelocitiesParallel();
        computeRightHandSideParallel();
        pressureSolver_->solve();
        computeVelocitiesParallel();
        exchangeVelocities();

        
        out.writeFile(currentTime);
        currentTime += dt_;

        #ifndef NDEBUG
        std::cout << "Time: " << currentTime << " and dt " << dt_ << std::endl;
        #endif



        // DEBUGGING 
        // std::string str = "Rank: " + std::to_string(communicator_->ownRankNo()) 
        //                              + " PartX" + std::to_string(partitioning_->nProcesses()[0]) 
        //                              + " PartY" + std::to_string(partitioning_->nProcesses()[1])
        //                              + " Zeit " + std::to_string(currentTime) 
        //                              + " dt"    + std::to_string(dt_);
        // printer_.add_new_parameter_to_print(str);

        // outputWriterParaviewParallel_->writeFile(currentTime);


    } while (currentTime < settings_.endTime);
    
    // TODO: remove prints
    printer_.save_values_to_file();
}










void ComputationParallel::computeTimeStepWidthParallel(double currentTime){
        // Diffusion operator (always > 0)
    double dx2 = discretization_->dx() * discretization_->dx();
    double dy2 = discretization_->dy() * discretization_->dy();
    double diff = settings_.re / 2 * (dx2 * dy2) / (dx2 + dy2);

    // convection operator restriction u
    double global_max_u = communicator_->getGlobalMax(discretization_->u().findAbsMax());
    double max_u_dt = discretization_->dx() / global_max_u;

    // convection operator restriction v
    double global_max_v = communicator_->getGlobalMax(discretization_->v().findAbsMax());
    double max_v_dt = discretization_->dy() / global_max_v;


    dt_ = settings_.tau * std::min({diff, max_u_dt, max_v_dt, settings_.maximumDt});


    // Print every whole second
    double nextWholeSecond = std::floor(currentTime) + 1;

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
    for (int i = f_i_beg; i < f_i_end; i++)
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
        for (int j = g_j_beg; j < g_j_end; j++)
        {
            double diffusion = 1 / settings_.re * (discretization_->computeD2vDx2(i, j) + discretization_->computeD2vDy2(i, j));
            double convection = -discretization_->computeDv2Dy(i, j) - discretization_->computeDuvDx(i, j);

            discretization_->g(i, j) = discretization_->v(i, j) + dt_ * (diffusion + convection + settings_.g[1]);
        }
    }  
}

void ComputationParallel::computeRightHandSideParallel(){

    int i_beg = discretization_->rhsIBegin();
    int i_end = discretization_->rhsIEnd();
    int j_beg = discretization_->rhsJBegin();
    int j_end = discretization_->rhsJEnd();

    // Interior
    for (int i = i_beg; i < i_end; i++)
    {
        for (int j = j_beg; j < j_end; j++)
        {
            // TODO: make the used knowledge of the staggered grid explicit, we go from rhs -> to f and g idx which are different
            // double dF = (1 / discretization_->dx()) * (discretization_->f(i, j) - discretization_->f(i - 1, j));
            // double dG = (1 / discretization_->dy()) * (discretization_->g(i, j) - discretization_->g(i, j - 1));
            double dF = (1 / discretization_->dx()) * (discretization_->f(i + 1, j) - discretization_->f(i, j));
            double dG = (1 / discretization_->dy()) * (discretization_->g(i, j + 1) - discretization_->g(i, j));
            discretization_->rhs(i, j) = (1 / dt_) * (dF + dG);
        }
    }

}


void ComputationParallel::computeVelocitiesParallel(){

    // TODO: Doppelrechnung - setzen Rand falsch, um dann BV zu setzen
    // wenn Rand dann doch eher uIBegin()

    // Copy from serial
    for (int i = discretization_->uLeftGhost() + 1; i < discretization_->uRightGhost() - 1; i++)
    {
        for (int j = discretization_->uJBegin() + 1; j < discretization_->uJEnd() - 1; j++)
        {
            discretization_->u(i, j) = discretization_->f(i, j) - dt_ * discretization_->computeDpDx(i, j);
        }
    }

    for (int i = discretization_->vIBegin() + 1; i < discretization_->vIEnd() - 1; i++)
    {
        for (int j = discretization_->vBottomGhost() + 1; j < discretization_->vTopGhost() - 1; j++)
        {
            discretization_->v(i, j) = discretization_->g(i, j) - dt_ * discretization_->computeDpDy(i, j);
        }
    }

}

void ComputationParallel::exchangeVelocities(){
    // TODO: Optimierung Setzt Ecken mit (man kann eigentlich noch v_i + 1 und v_end - 1 machen)
    // TODO: Nutze nur einen Buffer weil gleiche Größe
    if (!partitioning_->ownPartitionContainsBottomBoundary()){
        // u
        std::vector<double> buffer_u = discretization_->u().getRow(discretization_->uJBegin() + 1,
                                                        discretization_->uIBegin(),
                                                        discretization_->uIEnd());
        int buffer_u_size = discretization_->uIEnd() - discretization_->uIBegin();

        std::vector<double> buffer_receive_u = communicator_->receiveFrom(
                                partitioning_->bottomNeighbourRankNo(), buffer_u_size);
        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer_u);

        for (int i = discretization_->uIBegin(); i < discretization_->uIEnd(); i++){
            discretization_->u(i, discretization_->uJBegin()) = buffer_receive_u[i - discretization_->uIBegin()];
        }

        // v
        std::vector<double> buffer_v = discretization_->v().getRow(discretization_->vJBegin() + 1,
                                                        discretization_->vIBegin(),
                                                        discretization_->vIEnd());
        int buffer_v_size = discretization_->vIEnd() - discretization_->vIBegin();

        std::vector<double> buffer_receive_v = communicator_->receiveFrom(
                                partitioning_->bottomNeighbourRankNo(), buffer_v_size);
        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer_v);

        for (int i = discretization_->vIBegin(); i < discretization_->vIEnd(); i++){
            discretization_->v(i, discretization_->vBottomGhost()) = buffer_receive_v[i - discretization_->vIBegin()];
        }

    }   

    if (!partitioning_->ownPartitionContainsTopBoundary()){
        // u
        std::vector<double> buffer_u = discretization_->u().getRow(discretization_->uJEnd() - 2,
                                                        discretization_->uIBegin(),
                                                        discretization_->uIEnd());
        int buffer_u_size = discretization_->uIEnd() - discretization_->uIBegin();

        communicator_->sendTo(partitioning_->topNeighbourRankNo(), buffer_u);
        std::vector<double> buffer_receive_u = communicator_->receiveFrom(
                                partitioning_->topNeighbourRankNo(), buffer_u_size);

        for (int i = discretization_->uIBegin(); i < discretization_->uIEnd(); i++){
            discretization_->u(i, discretization_->uJEnd()-1) = buffer_receive_u[i - discretization_->uIBegin()];
        }

        // v
        std::vector<double> buffer_v = discretization_->v().getRow(discretization_->vJEnd() - 2,
                                                        discretization_->vIBegin(),
                                                        discretization_->vIEnd());
        int buffer_v_size = discretization_->vIEnd() - discretization_->vIBegin();

        communicator_->sendTo(partitioning_->topNeighbourRankNo(), buffer_v);
        std::vector<double> buffer_receive_v = communicator_->receiveFrom(
                                partitioning_->topNeighbourRankNo(), buffer_v_size);

        for (int i = discretization_->vIBegin(); i < discretization_->vIEnd(); i++){
            discretization_->v(i, discretization_->vTopGhost() - 1) = buffer_receive_v[i - discretization_->vIBegin()];
        }

    }   

    if (!partitioning_->ownPartitionContainsLeftBoundary()){
        // u
        std::vector<double> buffer_u = discretization_->u().getColumn(discretization_->uIBegin() + 1,
                                                        discretization_->uJBegin(),
                                                        discretization_->uJEnd());
        int buffer_u_size = discretization_->uJEnd() - discretization_->uJBegin();

        std::vector<double> buffer_receive_u = communicator_->receiveFrom(
                                partitioning_->leftNeighbourRankNo(), buffer_u_size);
        communicator_->sendTo(partitioning_->leftNeighbourRankNo(), buffer_u);

        for (int j = discretization_->uJBegin(); j < discretization_->uJEnd(); j++){
            discretization_->u(discretization_->uLeftGhost(), j) = buffer_receive_u[j - discretization_->uIBegin()];

        }
        // v
        std::vector<double> buffer_v = discretization_->v().getColumn(discretization_->vIBegin() + 1,
                                                        discretization_->vJBegin(),
                                                        discretization_->vJEnd());
        int buffer_v_size = discretization_->vJEnd() - discretization_->vJBegin();

        std::vector<double> buffer_receive_v = communicator_->receiveFrom(
                                partitioning_->leftNeighbourRankNo(), buffer_v_size);
        communicator_->sendTo(partitioning_->leftNeighbourRankNo(), buffer_v);

        for (int j = discretization_->vJBegin(); j < discretization_->vJEnd(); j++){
            discretization_->v(discretization_->vIBegin(), j) = buffer_receive_v[j - discretization_->vIBegin()];

        }

    }

    if (!partitioning_->ownPartitionContainsRightBoundary()){
        // u
        std::vector<double> buffer_u = discretization_->u().getColumn(discretization_->uIEnd() - 2,
                                                        discretization_->uJBegin(),
                                                        discretization_->uJEnd());
        int buffer_u_size = discretization_->uJEnd() - discretization_->uJBegin();

        communicator_->sendTo(partitioning_->rightNeighbourRankNo(), buffer_u);
        std::vector<double> buffer_receive_u = communicator_->receiveFrom(
                                partitioning_->rightNeighbourRankNo(), buffer_u_size);

        for (int j = discretization_->uJBegin(); j < discretization_->uJEnd(); j++){
            discretization_->u(discretization_->uRightGhost() - 1, j) = buffer_receive_u[j - discretization_->uIBegin()];

        }
        // v
        std::vector<double> buffer_v = discretization_->v().getColumn(discretization_->vIEnd() - 2,
                                                        discretization_->vJBegin(),
                                                        discretization_->vJEnd());
        int buffer_v_size = discretization_->vJEnd() - discretization_->vJBegin();

        communicator_->sendTo(partitioning_->rightNeighbourRankNo(), buffer_v);
        std::vector<double> buffer_receive_v = communicator_->receiveFrom(
                                partitioning_->rightNeighbourRankNo(), buffer_v_size);

        for (int j = discretization_->vJBegin(); j < discretization_->vJEnd(); j++){
            discretization_->v(discretization_->vIEnd() - 1, j) = buffer_receive_v[j - discretization_->vIBegin()];

        }

    }


    

}

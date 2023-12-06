#include "red_black.h"
#include "output_writer/output_writer_text_parallel.h"

RedBlack::RedBlack(const std::shared_ptr<Discretization> &data,
                         double epsilon,
                         int maximumNumberOfIterations,
                         std::shared_ptr<Communicator> communicator,
                         std::shared_ptr<Partitioning> partitioning): 
                PressureSolver(data, epsilon, maximumNumberOfIterations),
                communicator_(communicator),
                partitioning_(partitioning)
{
}

void RedBlack::solve()
{
    OutputWriterTextParallel out = OutputWriterTextParallel(discretization_, *partitioning_);
    setBoundaryValues();
    
    int n = 0;
    int currentModulo;
    double d_fac = (dx2 * dy2) / (2 * (dx2 + dy2));

    double res = sqrt(communicator_->getGlobalSum(calculateResiduum()));
     
    double omegaOpt = 2/(1 + std::sin(M_PI * (discretization_->dx() + discretization_->dy())/2));

    while (n < maximumNumberOfIterations_ && res > epsilon_) //
    {
        for (int redBlack=0; redBlack < 2; redBlack++){
            if (partitioning_->lowerLeftIsRed()){
                currentModulo = redBlack;
            } else {
                currentModulo = (redBlack+1)%2;
            }

            // out.writePressureFile();

            // TODO: Opti (kann man N^2/2 Schleifendurchläufe sparen?)
            for (int j = j_beg; j < j_end; j++)
            {
                int i_start = i_beg + (currentModulo + j)%2; // black first
                for (int i = i_start; i < i_end; i+=2)
                {

                    // std::cout << "i, j: " << i << j << std::endl;
                    double p_x = 1 / dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                    double p_y = 1 / dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1));

                    discretization_->p(i, j) = (1 - omegaOpt) * discretization_->p(i, j) + omegaOpt * (d_fac * (p_x + p_y - discretization_->rhs(i, j)));
                    
                }
            }
            exchangeGhost(currentModulo);
            setBoundaryValues();
        }

        // Compute the residual with new values
        res = sqrt(communicator_->getGlobalSum(calculateResiduum()));
        n++;
    }


#ifndef NDEBUG
    if (communicator_->ownRankNo() == 0){
        std::cout << "[Solver] Number of iterations: " << n << ", final residuum: " << res << std::endl;
        // std::cout << "\t" << "Omega Optimal: " << omegaOpt << std::endl;
    }
#endif
}


void RedBlack::setBoundaryValues(){


    if(partitioning_->ownPartitionContainsBottomBoundary()){
        for(int i = discretization_->pIBegin(); i < discretization_->pIEnd(); i++){
            discretization_->p(i,discretization_->pJBegin()) = discretization_->p(i,discretization_->pJBegin()+1);
        }
    }

    if(partitioning_->ownPartitionContainsTopBoundary()){
        for(int i = discretization_->pIBegin(); i < discretization_->pIEnd(); i++){
            discretization_->p(i,discretization_->pJEnd()-1) = discretization_->p(i,discretization_->pJEnd()-2);
        }
    }

    if(partitioning_->ownPartitionContainsLeftBoundary()){
        for(int j = discretization_->pJBegin(); j<discretization_->pJEnd(); j++){
            discretization_->p(discretization_->pIBegin(), j) = discretization_->p(discretization_->pIBegin()+1, j);
        }
    }

    if(partitioning_->ownPartitionContainsRightBoundary()){
        for(int j = discretization_->pJBegin(); j<discretization_->pJEnd(); j++){
            discretization_->p(discretization_->pIEnd()-1, j) = discretization_->p(discretization_->pIEnd()-2, j);
        }
    }

}

void RedBlack::exchangeGhost(int currentModulo){

    #ifndef NDEBUG
    std::cout << "Rank " << communicator_->ownRankNo()
              << " has lowerLeftIsRed=" << partitioning_->lowerLeftIsRed()
              << " , currentModulo=" << currentModulo
              << ", i_beg=" << i_beg << ", j_beg=" << j_beg
              << ", i_end=" << i_end << ", j_end=" << j_end
              << std::endl;
    #endif

    int step = 2; // step of getRow, getColumn
    bool exchange_half = ((step-1)%2); // if step==1, leave start=beg
    int buffer_size_i = int(std::ceil((i_end - i_beg)/step));
    int buffer_size_j = int(std::ceil((j_end - j_beg)/step));

    // TODO: Optimierung -> sende je nach Schritt nur die Hälfte der Daten
    if (!partitioning_->ownPartitionContainsBottomBoundary()){
        int i_start = i_beg + exchange_half*(currentModulo + 1)%2;

        #ifndef NDEBUG
        std::cout << "\t[Bottom] Rank " << communicator_->ownRankNo() << ", i_start=" << i_start << ", i_beg=" << i_beg << std::endl;
        #endif

        std::vector<double> buffer = discretization_->p().getRow(j_beg, i_start, i_end, step);
        std::vector<double> buffer_receive;

        buffer_receive = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(), buffer_size_i);
        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer);

        for (int i = i_start; i < i_end; i += step){
            discretization_->p(i, j_beg-1) = buffer_receive[(i - i_start)/step];
        }

    }

    if (!partitioning_->ownPartitionContainsTopBoundary()){
        int modulo_height = (j_end - j_beg)%2;
        int i_start = i_beg + exchange_half*(currentModulo + modulo_height)%2;

        #ifndef NDEBUG
        std::cout << "\t[  Top ] Rank " << communicator_->ownRankNo() << ", i_start=" << i_start << ", i_beg=" << i_beg << std::endl;
        #endif

        std::vector<double> buffer = discretization_->p().getRow(j_end - 1, i_start, i_end, step);
        std::vector<double> buffer_receive;

        communicator_->sendTo(partitioning_->topNeighbourRankNo(), buffer);
        buffer_receive = communicator_->receiveFrom(partitioning_->topNeighbourRankNo(), buffer_size_i);

        for (int i = i_start; i < i_end; i += step){
            discretization_->p(i, j_end) = buffer_receive[(i - i_start)/step];
        }
    }

    if (!partitioning_->ownPartitionContainsLeftBoundary()){
        int j_start = j_beg + exchange_half*(currentModulo + 1)%2;

        #ifndef NDEBUG
        std::cout << "\t[ Left ] Rank " << communicator_->ownRankNo() << ", j_start=" << j_start << ", j_beg=" << j_beg << std::endl;
        #endif

        std::vector<double> buffer = discretization_->p().getColumn(i_beg, j_start, j_end, step);
        std::vector<double> buffer_receive;

        buffer_receive = communicator_->receiveFrom(partitioning_->leftNeighbourRankNo(), buffer_size_j);
        communicator_->sendTo(partitioning_->leftNeighbourRankNo(), buffer);

        for (int j = j_start; j < j_end; j += step){
            discretization_->p(i_beg - 1, j) = buffer_receive[(j - j_start)/step];
        }
    }

        if (!partitioning_->ownPartitionContainsRightBoundary()){
        int modulo_width = (i_end - i_beg)%2;
        int j_start = j_beg + exchange_half*(currentModulo + modulo_width)%2;

        #ifndef NDEBUG
        std::cout << "\t[ Right] Rank " << communicator_->ownRankNo() << ", j_start=" << j_start << ", j_beg=" << j_beg << std::endl;
        #endif

        std::vector<double> buffer = discretization_->p().getColumn(i_end - 1, j_start, j_end, step);
        std::vector<double> buffer_receive;

        communicator_->sendTo(partitioning_->rightNeighbourRankNo(), buffer);
        buffer_receive = communicator_->receiveFrom(partitioning_->rightNeighbourRankNo(), buffer_size_j);

        for (int j = j_start; j < j_end; j += step){
            discretization_->p(i_end, j) = buffer_receive[(j - j_start)/step];
        }
    }

}

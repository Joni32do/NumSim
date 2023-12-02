#include "red_black.h"

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
    setBoundaryValues();
    
    

    int n = 0;
    int currentModulo;
    double res = epsilon_ + 1;

    double d_fac = (dx2 * dy2) / (2 * (dx2 + dy2));
    do
    {
        for (int redBlack=0; redBlack < 2; redBlack++){
            if (partitioning_->lowerLeftIsRed()){
                currentModulo = redBlack;
            } else {
                currentModulo = (redBlack+1)%2;
            }

            
            for (int i = i_beg; i < i_end; i++)
            {
                for (int j = j_beg; j < j_end; j++)
                {
                    if ((i + j)%2 == currentModulo){
                        double p_x = 1 / dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                        double p_y = 1 / dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1));

                        discretization_->p(i, j) = d_fac * (p_x + p_y - discretization_->rhs(i, j));
                    }
                    
                }
            }

            exchangeGhost();
        }
        setBoundaryValues();
        // Compute the residual with new values
        res = calculateResiduum();
        n++;


        
    } while (n < maximumNumberOfIterations_ && res > epsilon_);

#ifndef NDEBUG
    std::cout << "[Solver] Number of iterations: " << n << ", final residuum: " << res << std::endl;
#endif
}


void RedBlack::setBoundaryValues(){
    if(partitioning_->ownPartitionContainsBottomBoundary()){
        for(int i = discretization_->pIBegin(); i < discretization_->pIEnd(); i++){
            discretization_->p(i,discretization_->pJBegin()) = -discretization_->p(i,discretization_->pJBegin()+1);
        }
    }

    if(partitioning_->ownPartitionContainsTopBoundary()){
        for(int i = discretization_->pIBegin(); i < discretization_->pIEnd(); i++){
            discretization_->p(i,discretization_->pJEnd()-1) = -discretization_->p(i,discretization_->pJEnd()-2);
        }
    }

    if(partitioning_->ownPartitionContainsLeftBoundary()){
        for(int j = discretization_->pJBegin(); j<discretization_->pJEnd(); j++){
            discretization_->p(discretization_->pIBegin(), j) = -discretization_->p(discretization_->pIBegin()+1, j);
        }
    }

    if(partitioning_->ownPartitionContainsRightBoundary()){
        for(int j = discretization_->pJBegin(); j<discretization_->pJEnd(); j++){
            discretization_->p(discretization_->pIEnd()-1, j) = -discretization_->p(discretization_->pIBegin()-2, j);
        }
    }


}

void RedBlack::exchangeGhost(){
    // TODO: Optimierung -> sende je nach Schritt nur die Hälfte der Daten
    if (!partitioning_->ownPartitionContainsBottomBoundary()){
        std::vector<double> buffer = discretization_->p().getRow(j_beg, i_beg, i_end);
        std::vector<double> buffer_receive;
        int buffer_size = i_end - i_beg;

        buffer_receive = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(), buffer_size);
        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer);

        for (int i = i_beg; i < i_end; i++){
            discretization_->p(i, j_beg-1) = buffer_receive[i - i_beg];
        }

    }

    if (!partitioning_->ownPartitionContainsTopBoundary()){
        std::vector<double> buffer = discretization_->p().getRow(j_end - 1, i_beg, i_end);
        std::vector<double> buffer_receive;
        int buffer_size = i_end - i_beg;

        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer);
        buffer_receive = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(), buffer_size);

        for (int i = i_beg; i < i_end; i++){
            discretization_->p(i, j_end) = buffer_receive[i - i_beg];
        }
    }

    if (!partitioning_->ownPartitionContainsLeftBoundary()){
        std::vector<double> buffer = discretization_->p().getColumn(i_beg, j_beg, j_end);
        std::vector<double> buffer_receive;
        int buffer_size = j_end - j_beg;

        buffer_receive = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(), buffer_size);
        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer);

        for (int j = j_beg; j < j_end; j++){
            discretization_->p(i_beg - 1, j) = buffer_receive[j - j_beg];
        }
    }

        if (!partitioning_->ownPartitionContainsRightBoundary()){
        std::vector<double> buffer = discretization_->p().getColumn(i_end - 1, j_beg, j_end);
        std::vector<double> buffer_receive;
        int buffer_size = j_end - j_beg;

        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer);
        buffer_receive = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(), buffer_size);

        for (int j = j_beg; j < j_end; j++){
            discretization_->p(i_end, j) = buffer_receive[j - j_beg];
        }
    }

}

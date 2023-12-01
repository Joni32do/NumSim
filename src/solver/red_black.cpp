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
    double res = epsilon_ + 1;

    double d_fac = (dx2 * dy2) / (2 * (dx2 + dy2));
    do
    {
        for (int i = i_beg; i < i_end; i++)
        {
            for (int j = j_beg; j < j_end; j++)
            {

                double p_x = 1 / dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                double p_y = 1 / dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1));

                discretization_->p(i, j) = d_fac * (p_x + p_y - discretization_->rhs(i, j));
            }
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

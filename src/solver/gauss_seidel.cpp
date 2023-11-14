#include "gauss_seidel.h"


GaussSeidel::GaussSeidel(const std::shared_ptr<Discretization>& data, 
                         double epsilon, 
                         int maximumNumberOfIterations):
            PressureSolver(data, epsilon, maximumNumberOfIterations)
{
}


void GaussSeidel::solve(){
    setBoundaryValues();

    int n = 0;
    double res = epsilon_ + 1;

    double d_fac = (dx2 * dy2)/(2 *(dx2 + dy2));
    do {
        for (int i = i_beg; i < i_end; i++) {
            for (int j = j_beg; j < j_end; j++) {

                double p_x = 1/dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                double p_y = 1/dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1)); 

                discretization_->p(i,j) = d_fac * (p_x + p_y - discretization_->rhs(i, j));
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


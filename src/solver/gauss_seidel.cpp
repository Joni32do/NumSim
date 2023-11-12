#include "gauss_seidel.h"

#include <iostream>

GaussSeidel::GaussSeidel(const std::shared_ptr<Discretization>& data, 
                         double epsilon, 
                         int maximumNumberOfIterations,
                         std::array<double,2> meshWidth):
            PressureSolver(data, epsilon, maximumNumberOfIterations, meshWidth)
{
}


void GaussSeidel::solve(){
    setBoundaryValues();
    // TODO: double check the indicies
    int rhs_i_beg = discretization_->pIBegin() + 1;
    int rhs_i_end = discretization_->pIEnd() - 1;
    int rhs_j_beg = discretization_->pJBegin() + 1;
    int rhs_j_end = discretization_->pJEnd() - 1;
    
    int n = 0;
    double res = epsilon_ + 1;

    double dx2 = discretization_->dx() * discretization_->dx();
    double dy2 = discretization_->dy() * discretization_->dy();
    double d_fac = (dx2 * dy2)/(2 *(dx2 + dy2));
    int n_rhs = (rhs_i_end - rhs_i_beg + 1) * (rhs_j_end - rhs_j_beg + 1);

    // Is cpp so low level that loops are faster then any other matrix operation?
    do {
        for (int i = rhs_i_beg; i <= rhs_i_end; i++) {
            for (int j = rhs_j_beg; j <= rhs_j_end; j++) {
                double p_old = discretization_->p(i, j);
                double p_x = 1/dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                double p_y = 1/dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1)); 

                // Main SOR step
                double p_new = d_fac * (p_x + p_y - discretization_->rhs(i-1, j-1));

                discretization_->p(i,j) = p_new;
            }
        } 
        setBoundaryValues();
        // Compute the residual with new values
        res = calculateResiduum();

        // TODO: remove print
        // std::cout << "res: " << res << std::endl;
        n++;
    } while (n < maximumNumberOfIterations_ && res > epsilon_);

    #ifndef NDEBUG
        std::cout << "[Solver] Number of iterations: " << n << ", final residuum: " << res << std::endl;
    #endif
}


#include "gauss_seidel.h"

#include <iostream>

GaussSeidel::GaussSeidel(const std::shared_ptr<Discretization>& data, 
                         double epsilon, 
                         int maximumNumberOfIterations):
            PressureSolver(data, epsilon, maximumNumberOfIterations)
{
}


void GaussSeidel::solve(){
    setBoundaryValues();
    // TODO: double check the indicies
    int rhs_i_beg = discretization_->pIBegin();
    int rhs_i_end = discretization_->pIEnd();
    int rhs_j_beg = discretization_->pJBegin();
    int rhs_j_end = discretization_->pJEnd();
    
    int n = 0;
    double res = epsilon_ + 1;

    double dx2 = discretization_->dx() * discretization_->dx();
    double dy2 = discretization_->dy() * discretization_->dy();
    double d_fac = (dx2 * dy2)/(2 *(dx2 + dy2));
    int n_rhs = (rhs_i_end - rhs_i_beg) * (rhs_j_end - rhs_j_beg);

    // Is cpp so low level that loops are faster then any other matrix operation?
    do {
        res = 0;
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
        // 
        // Has to be done outside of the loop - therefore is NOT the same
        // as in loop above
        for (int i = rhs_i_beg; i <= rhs_i_end; i++) {
            for (int j = rhs_j_beg; j <= rhs_j_end; j++) {
                double p_old = discretization_->p(i, j);
                double p_x = 1/dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                double p_y = 1/dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1)); 
                double res_temp = d_fac * (p_x + p_y - discretization_->rhs(i, j));
                res += res_temp * res_temp;
            }
        } 
        res = std::sqrt(1/n_rhs * res);

        std::cout << "n_rhs: " << n_rhs << std::endl;
        std::cout << "res: " << res << std::endl;
        n++;
    } while (n < maximumNumberOfIterations_ && res > epsilon_);
}


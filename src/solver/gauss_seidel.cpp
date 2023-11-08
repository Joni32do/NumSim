#include "gauss_seidel.h"


GaussSeidel::GaussSeidel(const std::shared_ptr<Discretization>& data, 
                         double epsilon, 
                         int maximumNumberOfIterations):
            PressureSolver(data, epsilon, maximumNumberOfIterations)
{
}


void GaussSeidel::solve(){
    setBoundaryValues();

    int i_beg = discretization_->pIBegin();
    int i_end = discretization_->pIEnd();
    int j_beg = discretization_->pJBegin();
    int j_end = discretization_->pJEnd();
    
    int n = 0;
    double res = epsilon_ + 1;

    double dx2 = discretization_->dx() * discretization_->dx();
    double dy2 = discretization_->dy() * discretization_->dy();
    double d_fac = (dx2 * dy2)/(2 *(dx2 + dy2));

    // Can be implemented in a different manner to SOR but anyways
    while (n < maximumNumberOfIterations_ && res > epsilon_) {
        res = 0;
        for (int i = i_beg + 1; i < i_end - 1; i++) {
            for (int j = j_beg + 1; j < j_end - 1; j++) {
                double p_old = discretization_->p(i, j);
                double p_x = 1/dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                double p_y = 1/dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1)); 
                double p_new = p_old + (d_fac * (p_x + p_y - discretization_->rhs(i, j)));
                discretization_->p(i,j) = p_new;
                // TODO: Residual calculation with $1/N |r|^2 < e^2$ mit $r = rhs - M@p$ 
                res += std::abs(p_new - p_old);
            }
        }
        // Relative residual to discretization size ("integral")
        res /= (i_end - i_beg - 1) * (j_end - j_beg - 1);
        n++;
    }


}
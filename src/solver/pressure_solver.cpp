#include "pressure_solver.h"

PressureSolver::PressureSolver(std::shared_ptr<Discretization> discretization,
                               double epsilon, 
                               int maximumNumberOfIterations): 
            discretization_(discretization),
            epsilon_(epsilon),
            maximumNumberOfIterations_(maximumNumberOfIterations)
{
    assert(epsilon > 0);
    assert(maximumNumberOfIterations > 0);

}


void PressureSolver::setBoundaryValues(){

    // BV for p
    
    // According to lecture 3.2.3 $F_{0,j} = u_{0,j}$ results in Homogenous Neumann BC
    int i_beg = discretization_->pIBegin();
    int i_end = discretization_->pIEnd();
    int j_beg = discretization_->pJBegin();
    int j_end = discretization_->pJEnd();


    for(int i = i_beg + 1; i <= i_end - 1; i++){
        discretization_->p(i, j_beg) = discretization_->p(i, j_beg + 1);
        discretization_->p(i, j_end) = discretization_->p(i, j_end - 1);
    }

    for(int j = j_beg + 1; j <= j_end - 1; j++){
        discretization_->p(i_beg, j) = discretization_->p(i_beg + 1, j);
        discretization_->p(i_end, j) = discretization_->p(i_end - 1, j);
    }


}
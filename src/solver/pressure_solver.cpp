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
    // set homogeneous Neumann BC

    // BV for u
    int i_beg = discretization_->uIBegin();
    int i_end = discretization_->uIEnd();
    int j_beg = discretization_->uJBegin();
    int j_end = discretization_->uJEnd();

    for(int j = j_beg; j < j_end; j++){
        discretization_->u(i_beg, j) = 0;
        discretization_->u(i_end, j) = 0;
    }

    for(int i = i_beg; i < i_end; i++){
        discretization_->u(i, j_beg) = - discretization_->u(i, j_beg + 1);
        discretization_->u(i, j_end) = - discretization_->u(i, j_end - 1);
    }

    /**
     * Inhomogenous Neumann BC
     * 
     */

    // for(int i = i_beg; i < i_end; i++){
    //     discretization_->u(i, j_beg) = 2* u_b - discretization_->u(i, j_beg + 1);
    //     discretization_->u(i, j_end) = 2* u_b - discretization_->u(i, j_end - 1);
    // }
    

    // BV for v
    i_beg = discretization_->vIBegin();
    i_end = discretization_->vIEnd();
    j_beg = discretization_->vJBegin();
    j_end = discretization_->vJEnd();

    for(int i = i_beg; i < i_end; i++){
        discretization_->v(i, j_beg) = 0;
        discretization_->v(i, j_end) = 0;
    }

    for(int j = j_beg; j < j_end; j++){
        discretization_->v(i_beg, j) = - discretization_->v(i_beg + 1, j);
        discretization_->v(i_end, j) = - discretization_->v(i_end - 1, j);
    }

    // BV for p
    
    // According to lecture 3.2.3 $F_{0,j} = u_{0,j}$ results in Homogenous Neumann BC
    i_beg = discretization_->pIBegin();
    i_end = discretization_->pIEnd();
    j_beg = discretization_->pJBegin();
    j_end = discretization_->pJEnd();

    for(int i = i_beg; i < i_end; i++){
        discretization_->p(i, j_beg) = discretization_->p(i, j_beg + 1);
        discretization_->p(i, j_end) = discretization_->p(i, j_end - 1);
    }

    for(int j = j_beg; j < j_end; j++){
        discretization_->p(i_beg, j) = discretization_->p(i_beg + 1, j);
        discretization_->p(i_end, j) = discretization_->p(i_end - 1, j);
    }


}
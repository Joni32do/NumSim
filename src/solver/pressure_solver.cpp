#include "pressure_solver.h"
#include <math.h>
#include <iostream>

PressureSolver::PressureSolver(std::shared_ptr<Discretization> discretization,
                               double epsilon, 
                               int maximumNumberOfIterations,
                               std::array<double,2> meshWidth):
            discretization_(discretization),
            epsilon_(epsilon),
            maximumNumberOfIterations_(maximumNumberOfIterations),
            p(discretization->p()),
            rhs(discretization->rhs())
{
    assert(epsilon > 0);
    assert(maximumNumberOfIterations > 0);

    // loop boundaries
    i_beg = discretization_->pIBegin();
    i_end = discretization_->pIEnd();
    j_beg = discretization_->pJBegin();
    j_end = discretization_->pJEnd();

    // squared mesh widths
    dx2 = pow(meshWidth[0], 2);
    dy2 = pow(meshWidth[1], 2);
}


void PressureSolver::setBoundaryValues(){

    // BV for p
    
    // According to lecture 3.2.3 $F_{0,j} = u_{0,j}$ results in Homogenous Neumann BC
    // int i_beg = discretization_->pIBegin();
    // int i_end = discretization_->pIEnd();
    // int j_beg = discretization_->pJBegin();
    // int j_end = discretization_->pJEnd();

    // Vertical
    for(int j = j_beg; j <= j_end; j++){
        discretization_->p(i_beg, j) = discretization_->p(i_beg + 1, j);
        discretization_->p(i_end, j) = discretization_->p(i_end - 1, j);
    }

    // Horizontal (without conrners)
    for(int i = i_beg + 1; i <= i_end - 1; i++){
        discretization_->p(i, j_beg) = discretization_->p(i, j_beg + 1);
        discretization_->p(i, j_end) = discretization_->p(i, j_end - 1);
    }


}


double PressureSolver::calculateResiduum(){
    double pxx, pyy {0}; // 2nd derivative of p in x, y
    double res_current_point {0}; // residuum in a single point, to be added to sum of squares
    double sum_of_squares {0}; // to be applied in square root to yield internal product

    // int i_beg = discretization_->pIBegin();
    // int i_end = discretization_->pIEnd();
    // int j_beg = discretization_->pJBegin();
    // int j_end = discretization_->pJEnd();

    int N {(j_end - j_beg) * (i_end - i_beg)}; // number of points in p grid

    for (int i = i_beg+1; i < i_end-1; i++){
        for (int j = j_beg+1; j < j_end-1; j++){
            pxx = (p(i-1,j) - 2*p(i,j) + p(i+1, j)) / dx2;
            pyy = (p(i,j-1) - 2*p(i,j) + p(i, j+1)) / dy2;
            res_current_point = pxx + pyy - rhs(i,j);
            sum_of_squares += pow(res_current_point, 2);
        }
    }
    return sqrt(sum_of_squares/N);
}
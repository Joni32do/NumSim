#include "boundary.h"

void Boundary::updateBoundaryPressure(){
    updateBoundaryPressureDomain();
    updateBoundaryPressureObstacle();
    updateBoundaryPressureSurfaceTension();
}


void Boundary::updateBoundaryPressureDomain(){
    //  for (int i = i_beg; i < i_end; i++){
    //     discretization_->p(i, j_beg - 1) = discretization_->p(i, j_beg);
    //     discretization_->p(i, j_end) = discretization_->p(i, j_end - 1);
    // }

    // // Vertical (without corners)
    // for (int j = j_beg - 1; j < j_end + 1; j++){
    //     discretization_->p(i_beg - 1, j) = discretization_->p(i_beg, j);
    //     discretization_->p(i_end, j) = discretization_->p(i_end - 1, j);
    // }
}
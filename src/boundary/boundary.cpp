#include "boundary.h"

Boundary::Boundary(std::shared_ptr<Mask> mask,
                   std::shared_ptr<Discretization> discretization,
                   Settings settings) : mask_(mask), discretization_(discretization), settings_(settings)
{

    for (int i = 0; i < mask_->size()[0]; i++)
    {
        for (int j = 0; j < mask_->size()[1]; j++)
        {
            if (mask_->isBorder(i, j))
            {
                boundaryCells_.push_back(i + j * mask_->size()[0]);
            }
        }
    }

}

void Boundary::setPressureBoundaryValues(){
    for (int idx : boundaryCells_)
    {
        int i = idx % mask_->size()[0];
        int j = idx / mask_->size()[0];

        if (mask_->isFluid(i, j))
        {
            setPressureBoundarySurface(i, j);
        }
        else if (mask_->isObstacle(i, j))
        {
            setPressureBoundaryObstacle(i, j);
        }
    }
}

void Boundary::setPressureBoundarySurface(int i, int j){
    //TODO: Implement
}

void Boundary::setPressureBoundaryObstacle(int i, int j){
    // switch((*mask_)(i, j)){
    //     case Mask::OBSTACLE_BORDER_TOP:
    //         discretization_->p(i, j) = discretization_->p(i, j + 1);
    //         break;
    //     case Mask::OBSTACLE_BORDER_BOTTOM:
    //         discretization_->p(i, j) = discretization_->p(i, j - 1);
    //         break;
    //     case Mask::OBSTACLE_BORDER_LEFT:
    //         discretization_->p(i, j) = discretization_->p(i - 1, j);
    //         break;
    //     case Mask::OBSTACLE_BORDER_RIGHT:
    //         discretization_->p(i, j) = discretization_->p(i + 1, j);
    //         break;
    // }


    
    if ((*mask_)(i, j) == Mask::OBSTACLE_BORDER_TOP){
        discretization_->p(i, j) = discretization_->p(i, j + 1);
    }
    else if ((*mask_)(i, j) == Mask::OBSTACLE_BORDER_BOTTOM){
        discretization_->p(i, j) = discretization_->p(i, j - 1);
    }
    else if ((*mask_)(i, j) == Mask::OBSTACLE_BORDER_LEFT){
        discretization_->p(i, j) = discretization_->p(i - 1, j);
    }
    else if ((*mask_)(i, j)== Mask::OBSTACLE_BORDER_RIGHT){
        discretization_->p(i, j) = discretization_->p(i + 1, j);
    }
    // else if (mask_->(i, j) == OBSTACLE_CORNER_BOTTOM_LEFT){
    //     discretization_->p(i, j) = 0.5 * (discretization_->p(i, j - 1) + discretization_->p(i - 1, j));
    // }
    // else if (mask_->(i, j) == OBSTACLE_CORNER_TOP_LEFT){
    //     discretization_->p(i, j) = 0.5 * (discretization_->p(i, j + 1) + discretization_->p(i - 1, j));
    // }
    // else if (mask_->(i, j) == OBSTACLE_CORNER_TOP_RIGHT){
    //     discretization_->p(i, j) = 0.5 * (discretization_->p(i, j + 1) + discretization_->p(i + 1, j));
    // }
    // else if (mask_->(i, j) == OBSTACLE_CORNER_BOTTOM_RIGHT){
    //     discretization_->p(i, j) = 0.5 * (discretization_->p(i, j - 1) + discretization_->p(i + 1, j));
    // }
    // else{
    //     std::cout << "Error: Boundary condition not implemented" << std::endl;
    // }
}




void Boundary::setVelocityBoundaryValues(){
    setUBoundaryValues();
    setVBoundaryValues();
}

void Boundary::setUBoundaryValues(){
    // BV for u
    
    int i_beg = discretization_->uIBegin();
    int i_end = discretization_->uIEnd();
    int j_beg = discretization_->uJBegin();
    int j_end = discretization_->uJEnd();

    // Vertical
    for (int j = j_beg; j < j_end; j++)
    {
        discretization_->u(i_beg, j) = settings_.dirichletBcLeft[0];
        discretization_->u(i_end - 1, j) = settings_.dirichletBcRight[0];
    }
    // Horizontal (leave out corners)
    for (int i = i_beg + 1; i < i_end - 1; i++)
    {
        discretization_->u(i, j_beg) = 2 * settings_.dirichletBcBottom[0] - discretization_->u(i, j_beg + 1);
        discretization_->u(i, j_end - 1) = 2 * settings_.dirichletBcTop[0] - discretization_->u(i, j_end - 2);
    }
}

void Boundary::setVBoundaryValues(){
    // BV for v
    int i_beg = discretization_->vIBegin();
    int i_end = discretization_->vIEnd();
    int j_beg = discretization_->vJBegin();
    int j_end = discretization_->vJEnd();

    // Vertical
    for (int j = j_beg; j < j_end; j++)
    {
        discretization_->v(i_beg, j) = 2 * settings_.dirichletBcLeft[1] - discretization_->v(i_beg + 1, j);
        discretization_->v(i_end - 1, j) = 2 * settings_.dirichletBcRight[1] - discretization_->v(i_end - 2, j);
    }
    // Horizontal (leave out corners)
    for (int i = i_beg + 1; i < i_end - 1; i++)
    {
        discretization_->v(i, j_beg) = settings_.dirichletBcBottom[1];
        discretization_->v(i, j_end - 1) = settings_.dirichletBcTop[1];
    }
}

















// void Boundary::updateBoundaryPressureDomain(){
//     //  for (int i = i_beg; i < i_end; i++){
//     //     discretization_->p(i, j_beg - 1) = discretization_->p(i, j_beg);
//     //     discretization_->p(i, j_end) = discretization_->p(i, j_end - 1);
//     // }

//     // // Vertical (without corners)
//     // for (int j = j_beg - 1; j < j_end + 1; j++){
//     //     discretization_->p(i_beg - 1, j) = discretization_->p(i_beg, j);
//     //     discretization_->p(i_end, j) = discretization_->p(i_end - 1, j);
//     // }
// }P
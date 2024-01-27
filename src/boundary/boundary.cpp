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
    #ifndef NDEBUG
        mask_->printMask();
    #endif
}

int Boundary::getNumberOfBoundaryCells() const {
    return boundaryCells_.size();
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

// its important that first the velocities are updated
void Boundary::setPressureBoundarySurface(int i, int j){
    // u and v must be updated before p
    switch((*mask_)(i, j)){
        case Mask::FLUID_BORDER_LEFT:
            break;
        case Mask::FLUID_BORDER_TOP:
            break;
        case Mask::FLUID_BORDER_RIGHT:
            discretization_->p(i, j) = 2/settings_.re * ( discretization_->u(i, j) - discretization_->u(i - 1, j))/discretization_->dx();
            break;
        case Mask::FLUID_BORDER_BOTTOM:
            discretization_->p(i, j) = 2/settings_.re * ( discretization_->v(i, j) - discretization_->v(i, j - 1))/discretization_->dy();
            break;
    }
}

void Boundary::setPressureBoundaryObstacle(int i, int j){
    switch((*mask_)(i, j)){
        case Mask::DOMAIN_LEFT:
            discretization_->p(i, j) = discretization_->p(i + 1, j);
            break;
        case Mask::DOMAIN_TOP:
            discretization_->p(i, j) = discretization_->p(i, j - 1);
            break;
        case Mask::DOMAIN_RIGHT:
            discretization_->p(i, j) = discretization_->p(i - 1, j);
            break;
        case Mask::DOMAIN_BOTTOM:
            discretization_->p(i, j) = discretization_->p(i, j + 1);
            break;
    }
}

void Boundary::setVelocityBoundaryValues(){
    for (int idx : boundaryCells_)
    {
        int i = idx % mask_->size()[0];
        int j = idx / mask_->size()[0];

        if (mask_->isFluid(i, j))
        {
            // setVelocityBoundarySurfaceU(i, j);
            // setVelocityBoundarySurfaceV(i, j);
        }
        else if (mask_->isObstacle(i, j))
        {
            setVelocityBoundaryObstacleU(i, j);
            setVelocityBoundaryObstacleV(i, j);
        }
    }
    
}

void Boundary::setVelocityBoundarySurfaceU(int i, int j){
    // important that this is solved 
    //     from left to right (increasing i)
    //     from bottom to top (increasing j)
    double dxByDy = discretization_->dx() / discretization_->dy();

    switch ((*mask_)(i, j)) {
        case Mask::FLUID_BORDER_LEFT:
            discretization_->u(i, j);
            break;
        case Mask::FLUID_BORDER_TOP:
            break;
        case Mask::FLUID_BORDER_RIGHT:
            discretization_->u(i, j) = discretization_->u(i - 1, j)
                - dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));
            break;
        
        default:
            break;
    }
    
}

void Boundary::setVelocityBoundarySurfaceV(int i, int j){
    // TODO:
}



void Boundary::setVelocityBoundaryObstacleU(int i, int j){
    switch ((*mask_)(i, j))
    {
        case Mask::DOMAIN_LEFT:
            discretization_->u(i, j) = settings_.dirichletBcLeft[0];
            break;
        case Mask::DOMAIN_TOP:
            discretization_->u(i, j) = 2 * settings_.dirichletBcTop[0] - discretization_->u(i, j - 1);
            break;
        case Mask::DOMAIN_RIGHT:
            discretization_->u(i - 1, j) = settings_.dirichletBcRight[0]; // index shift for u in x direction
            break;
        case Mask::DOMAIN_BOTTOM:
            discretization_->u(i, j) = 2 * settings_.dirichletBcBottom[0] - discretization_->u(i, j + 1);
            break;
    }

    // int i_beg = discretization_->uIBegin();
    // int i_end = discretization_->uIEnd();
    // int j_beg = discretization_->uJBegin();
    // int j_end = discretization_->uJEnd();

    // // Vertical
    // for (int j = j_beg; j < j_end; j++)
    // {
    //     discretization_->u(i_beg, j) = settings_.dirichletBcLeft[0];
    //     discretization_->u(i_end - 1, j) = settings_.dirichletBcRight[0];
    // }
    // // Horizontal (leave out corners)
    // for (int i = i_beg + 1; i < i_end - 1; i++)
    // {
    //     discretization_->u(i, j_beg) = 2 * settings_.dirichletBcBottom[0] - discretization_->u(i, j_beg + 1);
    //     discretization_->u(i, j_end - 1) = 2 * settings_.dirichletBcTop[0] - discretization_->u(i, j_end - 2);
    // }

}

void Boundary::setVelocityBoundaryObstacleV(int i, int j){

    switch ((*mask_)(i, j))
    {
        case Mask::DOMAIN_LEFT:
            discretization_->v(i, j) = 2 * settings_.dirichletBcLeft[1] - discretization_->v(i + 1, j);
            break;
        case Mask::DOMAIN_TOP:
            discretization_->v(i, j - 1) = settings_.dirichletBcTop[1]; // index shift for v in y direction
            break;
        case Mask::DOMAIN_RIGHT:
            discretization_->v(i, j) = 2 * settings_.dirichletBcRight[1] - discretization_->v(i - 1, j);
            break;
        case Mask::DOMAIN_BOTTOM:
            discretization_->v(i, j) = settings_.dirichletBcBottom[1];
            break;
    }
    // // Vertical
    // for (int j = j_beg; j < j_end; j++)
    // {
    //     discretization_->v(i_beg, j) = 2 * settings_.dirichletBcLeft[1] - discretization_->v(i_beg + 1, j);
    //     discretization_->v(i_end - 1, j) = 2 * settings_.dirichletBcRight[1] - discretization_->v(i_end - 2, j);
    // }
    // // Horizontal (leave out corners)
    // for (int i = i_beg + 1; i < i_end - 1; i++)
    // {
    //     discretization_->v(i, j_beg) = settings_.dirichletBcBottom[1];
    //     discretization_->v(i, j_end - 1) = settings_.dirichletBcTop[1];
    // }
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
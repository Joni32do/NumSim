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
    double dx = discretization_->dx();
    double dy = discretization_->dy();
    switch((*mask_)(i, j)){
        case Mask::FLUID_BORDER_LEFT:
            discretization_->p(i, j) = 2/settings_.re*(discretization_->u(i, j) - discretization_->u(i - 1, j))/dx;
            break;
        case Mask::FLUID_BORDER_TOP:
            discretization_->p(i, j) = 2/settings_.re*(discretization_->v(i, j) - discretization_->v(i, j - 1))/dy;
            break;
        case Mask::FLUID_BORDER_RIGHT:
            discretization_->p(i, j) = 2/settings_.re*(discretization_->u(i, j) - discretization_->u(i - 1, j))/dx;
            break;
        case Mask::FLUID_BORDER_BOTTOM:
            discretization_->p(i, j) = 2/settings_.re*(discretization_->v(i, j) - discretization_->v(i, j - 1))/dy;
            break;
        case Mask::FLUID_CORNER_TOP_LEFT:
            discretization_->p(i, j) = -1/(2 * settings_.re) * (
                1/(2*dy) * (discretization_->u(i, j) + discretization_->u(i - 1, j) 
                    +discretization_->u(i, j - 1) - discretization_->u(i - 1, j - 1))
              + 1/(2*dx) * (discretization_->v(i, j) + discretization_->v(i, j - 1) 
                    +discretization_->v(i - 1, j) - discretization_->v(i - 1, j - 1))
            );
            break;
        case Mask::FLUID_CORNER_TOP_RIGHT:
            discretization_->p(i, j) = 1/(2 * settings_.re) * (
                1/(2*dy) * (discretization_->u(i, j) + discretization_->u(i - 1, j) 
                    +discretization_->u(i, j - 1) - discretization_->u(i - 1, j - 1))
              + 1/(2*dx) * (discretization_->v(i, j) + discretization_->v(i, j - 1) 
                    +discretization_->v(i - 1, j) - discretization_->v(i - 1, j - 1))
            );
            break;
        case Mask::FLUID_CORNER_BOTTOM_RIGHT:
            discretization_->p(i, j) =-1/(2 * settings_.re) * (
                1/(2*dy) * (discretization_->u(i, j) + discretization_->u(i - 1, j) 
                    +discretization_->u(i, j - 1) - discretization_->u(i - 1, j - 1))
              + 1/(2*dx) * (discretization_->v(i, j) + discretization_->v(i, j - 1) 
                    +discretization_->v(i - 1, j) - discretization_->v(i - 1, j - 1))
            );
            break;
        case Mask::FLUID_CORNER_BOTTOM_LEFT:
            discretization_->p(i, j) = 1/(2 * settings_.re) * (
                1/(2*dy) * (discretization_->u(i, j) + discretization_->u(i - 1, j) 
                    +discretization_->u(i, j - 1) - discretization_->u(i - 1, j - 1))
              + 1/(2*dy) * (discretization_->v(i, j) + discretization_->v(i, j - 1) 
                    +discretization_->v(i - 1, j) - discretization_->v(i - 1, j - 1))
            );
            break;
        case Mask::FLUID_COLUMN_VERTICAL:
            discretization_->p(i, j) = 2/settings_.re * (discretization_->u(i, j) - discretization_->u(i - 1, j))/dx;
            break;
        case Mask::FLUID_COLUMN_HORIZONTAL:
            discretization_->p(i, j) = 2/settings_.re * (discretization_->v(i, j) - discretization_->v(i, j - 1))/dy;
            break;
        case Mask::FLUID_SINGLE_LEFT:
            discretization_->p(i, j) = 0;
            break;
        case Mask::FLUID_SINGLE_TOP:
            discretization_->p(i, j) = 0;
            break;
        case Mask::FLUID_SINGLE_RIGHT:
            discretization_->p(i, j) = 0;
            break;
        case Mask::FLUID_SINGLE_BOTTOM:
            discretization_->p(i, j) = 0;
            break;
        case Mask::FLUID_DROPLET:
            discretization_->p(i, j) = 0;
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

void Boundary::setVelocityBoundarySurface(int i, int j, double dt){
    // important that this is solved 
    //     from left to right (increasing i)
    //     from bottom to top (increasing j)
    double dxByDy = discretization_->dx() / discretization_->dy();
    double dyByDx = discretization_->dy() / discretization_->dx();

    // Should the dt update step happen here? Then per loop only one execution is needed
    
    // Could be done way more cleaner, but I wanted to make it explicit first and not get abstaction get in the way of clarity
    // CE -> 

    switch ((*mask_)(i, j)) {
        case Mask::FLUID_BORDER_LEFT:
            // u
            discretization_->u(i - 1, j) = discretization_->u(i, j) 
                + dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));

            // v
            discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1)
                + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));
            break;

        case Mask::FLUID_BORDER_TOP:
            // v
            discretization_->v(i, j) = discretization_->v(i, j - 1)
                + dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));

            // u
            discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j)
                - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
            break;
        case Mask::FLUID_BORDER_RIGHT:
            // u
            discretization_->u(i, j) = discretization_->u(i - 1, j)
                - dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));

            // v
            discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1)
                + dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
            break;
        case Mask::FLUID_BORDER_BOTTOM:
            // v
            discretization_->v(i, j - 1) = discretization_->v(i, j)
                + dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));
            
            // u
            discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j)
                + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
            break;
        case Mask::FLUID_CORNER_TOP_LEFT:
            // u
            discretization_->u(i - 1, j) = discretization_->u(i, j);
            // v
            discretization_->v(i, j) = discretization_->v(i, j - 1);
            discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1)
                + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));
            // Overwrites also Boundary and Obstacle Cells just not if other water cells
            if (mask_->isAir(i - 1, j + 1))
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j); // chain, u(i, j) is equvivalent
                discretization_->v(i - 1, j) = discretization_->v(i, j);
            break;
        case Mask::FLUID_CORNER_TOP_RIGHT:
            // u
            discretization_->u(i, j) = discretization_->u(i - 1, j);
            discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j)
                - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
            // v
            discretization_->v(i, j) = discretization_->v(i, j - 1);
            discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1)
                - dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
            if (mask_->isAir(i + 1, j + 1)){
                discretization_->u(i, j + 1) = discretization_->u(i, j);
                discretization_->v(i + 1, j) = discretization_->v(i, j);
            } break;
        case Mask::FLUID_CORNER_BOTTOM_RIGHT:
            // u
            discretization_->u(i, j) = discretization_->u(i - 1, j);
            discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j)
                + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
            // v
            discretization_->v(i, j - 1) = discretization_->v(i, j);
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1);
                discretization_->u(i, j - 1) = discretization_->u(i, j);
            } break;
        case Mask::FLUID_CORNER_BOTTOM_LEFT:
            // u
            discretization_->u(i - 1, j) = discretization_->u(i, j);
            // v
            discretization_->v(i, j - 1) = discretization_->v(i, j);
            // Dont overwrite other fluid 
            if (mask_->isAir(i - 1, j - 1))
            {
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j);
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1);
                
            } break;
        case Mask::FLUID_COLUMN_HORIZONTAL:
            // v
            discretization_->v(i, j) = discretization_->v(i, j) + dt * settings_.g[1];
            discretization_->v(i, j - 1) = discretization_->v(i, j - 1) + dt * settings_.g[1];
            // u
            if (mask_->isAir(i - 1, j + 1)){
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j) 
                    - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
            }
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j) 
                    + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
            }
            break;
        case Mask::FLUID_COLUMN_VERTICAL:
            // u
            discretization_->u(i, j) = discretization_->u(i, j) + dt * settings_.g[0];
            discretization_->u(i - 1, j) = discretization_->u(i - 1, j) + dt * settings_.g[0];

            if (mask_->isAir(i - 1, j - 1)){
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1) 
                    + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));
            }
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1) 
                    - dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
            }
            break;
        case Mask::FLUID_SINGLE_LEFT:
            // v
            discretization_->v(i, j) = discretization_->v(i, j) + dt * settings_.g[1];
            discretization_->v(i, j - 1) = discretization_->v(i, j - 1) + dt * settings_.g[1];
            // u
            discretization_->u(i - 1, j) = discretization_->u(i, j)
                + dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));
            // others
            if (mask_->isAir(i - 1, j + 1)){
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j);
                discretization_->v(i - 1, j) = discretization_->v(i, j);
            }
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j);
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1);
            }
            break;
        case Mask::FLUID_SINGLE_TOP:
            // u
            discretization_->u(i, j) = discretization_->u(i, j) + dt * settings_.g[0];
            discretization_->u(i - 1, j) = discretization_->u(i - 1, j) + dt * settings_.g[0];
            // v
            discretization_->v(i, j) = discretization_->v(i, j - 1)
                - dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));
            // others
            if (mask_->isAir(i - 1, j + 1)){
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j);
                discretization_->v(i - 1, j) = discretization_->v(i, j);
            }
            if (mask_->isAir(i + 1, j + 1)){
                discretization_->u(i, j + 1) = discretization_->u(i, j);
                discretization_->v(i + 1, j) = discretization_->v(i, j);
            }

            // Tangential stress condition
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1)
                    + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));
            }
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1)
                    - dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
            }
            break;
        case Mask::FLUID_SINGLE_RIGHT:
            // v
            discretization_->v(i, j) = discretization_->v(i, j) + dt * settings_.g[1];
            discretization_->v(i, j - 1) = discretization_->v(i, j - 1) + dt * settings_.g[1];

            // u
            discretization_->u(i, j) = discretization_->u(i - 1, j)
                - dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));
            // others
            if (mask_->isAir(i + 1, j + 1)){
                discretization_->u(i, j + 1) = discretization_->u(i, j);
                discretization_->v(i + 1, j) = discretization_->v(i, j);
            }
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->u(i + 1, j - 1) = discretization_->u(i, j);
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1);
            }

            // Tangential stress condition
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j)
                    + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
            }
            if (mask_->isAir(i - 1, j + 1)){
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j)
                    - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
            }

            break;
        case Mask::FLUID_SINGLE_BOTTOM:
            // u
            discretization_->u(i, j) = discretization_->u(i, j) + dt * settings_.g[0];
            discretization_->u(i - 1, j) = discretization_->u(i - 1, j) + dt * settings_.g[0];
            // v
            discretization_->v(i, j - 1) = discretization_->v(i, j)
                + dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));
            // others
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j);
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1);
            }
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->u(i, j - 1) = discretization_->u(i, j);
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1);
            }
        case Mask::FLUID_DROPLET:
            // u
            discretization_->u(i, j) = discretization_->u(i, j) + dt * settings_.g[0];
            discretization_->u(i - 1, j) = discretization_->u(i - 1, j) + dt * settings_.g[0];
            // v
            discretization_->v(i, j) = discretization_->v(i, j) + dt * settings_.g[1];
            discretization_->v(i, j - 1) = discretization_->v(i, j - 1) + dt * settings_.g[1];

            // others
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j);
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1);
            }
            if (mask_->isAir(i - 1, j + 1)){
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j);
                discretization_->v(i - 1, j) = discretization_->v(i, j);
            }
            if (mask_->isAir(i + 1, j + 1)){
                discretization_->u(i, j + 1) = discretization_->u(i, j);
                discretization_->v(i + 1, j) = discretization_->v(i, j);
            }
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->u(i, j - 1) = discretization_->u(i, j);
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1);
            }
            break;
        default:
            break;
    }
    
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
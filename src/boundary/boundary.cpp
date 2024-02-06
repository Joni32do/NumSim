#include "boundary.h"

Boundary::Boundary(std::shared_ptr<Mask> mask,
                   std::shared_ptr<Discretization> discretization,
                   Settings settings) : mask_(mask), discretization_(discretization), settings_(settings)
{
    createBoundaryCellsLists();
}

void Boundary::createBoundaryCellsLists()
{
    for (int i = 0; i < mask_->size()[0]; i++)
    {
        for (int j = 0; j < mask_->size()[1]; j++)
        {
            int idx = i + j * mask_->size()[0];
            if (mask_->isDomainBoundary(i, j))
                domainBoundaryCells_.push_back(idx);
            if (mask_->isObstacleBoundary(i, j))
                obstacleBoundaryCells_.push_back(idx);
            if (mask_->isFluidBoundary(i, j))
                fluidBoundaryCells_.push_back(idx);
        }
    }
}

void Boundary::setPressureBoundaryValues()
{
    setPressureSurfaceBC();
    setPressureDomainBC();
    setPressureObstacleBC();
}

void Boundary::updateBoundary(){
    fluidBoundaryCells_ = {};
    for (int i = 0; i < mask_->size()[0]; i++) {
        for (int j = 0; j < mask_->size()[1]; j++) {
            int idx = i + j * mask_->size()[0];
            if (mask_->isFluidBoundary(i, j))
                fluidBoundaryCells_.push_back(idx);
        }
    }
}

void Boundary::setPressureDomainBC()
{
    for (int idx : domainBoundaryCells_)
    {
        int i = idx % mask_->size()[0];
        int j = idx / mask_->size()[0];

        switch ((*mask_)(i, j))
        {
        case Mask::DOMAIN_LEFT_NOSLIP:
            discretization_->p(i, j) = discretization_->p(i + 1, j);
            break;
        case Mask::DOMAIN_TOP_NOSLIP:
            discretization_->p(i, j) = discretization_->p(i, j - 1);
            break;
        case Mask::DOMAIN_RIGHT_NOSLIP:
            discretization_->p(i, j) = discretization_->p(i - 1, j);
            break;
        case Mask::DOMAIN_BOTTOM_NOSLIP:
            discretization_->p(i, j) = discretization_->p(i, j + 1);
            break;
        case Mask::DOMAIN_LEFT_PRESSURE:
            discretization_->p(i, j) = settings_.PressureLeft;
            break;
        case Mask::DOMAIN_TOP_PRESSURE:
            discretization_->p(i, j) = settings_.PressureTop;
            break;
        case Mask::DOMAIN_RIGHT_PRESSURE:
            discretization_->p(i, j) = settings_.PressureRight;
            break;
        case Mask::DOMAIN_BOTTOM_PRESSURE:
            discretization_->p(i, j) = settings_.PressureBottom;
            break;
        }
    }
}

void Boundary::setPressureObstacleBC()
{
    for (int idx : obstacleBoundaryCells_)
    {
        int i = idx % mask_->size()[0];
        int j = idx / mask_->size()[0];
        switch ((*mask_)(i, j))
        {
        case Mask::OBSTACLE_BORDER_LEFT:
            discretization_->p(i, j) = discretization_->p(i - 1, j);
            break;
        case Mask::OBSTACLE_BORDER_TOP:
            discretization_->p(i, j) = discretization_->p(i, j + 1);
            break;
        case Mask::OBSTACLE_CORNER_TOP_LEFT:
            discretization_->p(i, j) = 0.5 * (discretization_->p(i - 1, j) + discretization_->p(i, j + 1));
            break;
        case Mask::OBSTACLE_BORDER_RIGHT:
            discretization_->p(i, j) = discretization_->p(i + 1, j);
            break;
        case Mask::OBSTACLE_CORNER_TOP_RIGHT:
            discretization_->p(i, j) = 0.5 * (discretization_->p(i + 1, j) + discretization_->p(i, j + 1));
            break;
        case Mask::OBSTACLE_BORDER_BOTTOM:
            discretization_->p(i, j) = discretization_->p(i, j - 1);
            break;
        case Mask::OBSTACLE_CORNER_BOTTOM_RIGHT:
            discretization_->p(i, j) = 0.5 * (discretization_->p(i + 1, j) + discretization_->p(i, j - 1));
            break;
        case Mask::OBSTACLE_CORNER_BOTTOM_LEFT:
            discretization_->p(i, j) = 0.5 * (discretization_->p(i - 1, j) + discretization_->p(i, j - 1));
            break;
        }
    }
}

void Boundary::setPressureSurfaceBC()
{
    double dx = discretization_->dx();
    double dy = discretization_->dy();

    for (int idx : fluidBoundaryCells_)
    {
        int i = idx % mask_->size()[0];
        int j = idx / mask_->size()[0];
        

    // u and v must be updated before p

    // NORMAL STRESS
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
}

void Boundary::setVelocityBoundaryValues()
{
    setVelocitySurfaceBC();
    setVelocityDomainBC();
    setVelocityObstacleBC();
}

void Boundary::setVelocityBoundaryValues(double dt){
    setVelocitySurfaceBC(dt);
    setVelocityDomainBC();
    setVelocityObstacleBC();
}

void Boundary::setVelocityDomainBC()
{
    for (int idx : domainBoundaryCells_)
    {
        int i = idx % mask_->size()[0];
        int j = idx / mask_->size()[0];
        switch ((*mask_)(i, j))
        {
        case Mask::DOMAIN_LEFT_NOSLIP:
            discretization_->u(i, j) = settings_.NoSlipVelLeft[0];
            discretization_->f(i, j) = discretization_->u(i, j);
            discretization_->v(i, j) = 2 * settings_.NoSlipVelLeft[1] - discretization_->v(i + 1, j);
            break;
        case Mask::DOMAIN_TOP_NOSLIP:
            discretization_->u(i, j) = 2 * settings_.NoSlipVelTop[0] - discretization_->u(i, j - 1);
            discretization_->v(i, j - 1) = settings_.NoSlipVelTop[1];
            discretization_->g(i, j - 1) = discretization_->v(i, j - 1);
            break;
        case Mask::DOMAIN_RIGHT_NOSLIP:
            discretization_->u(i - 1, j) = settings_.NoSlipVelRight[0];
            discretization_->f(i - 1, j) = discretization_->u(i - 1, j);
            discretization_->v(i, j) = 2 * settings_.NoSlipVelRight[1] - discretization_->v(i - 1, j);
            break;
        case Mask::DOMAIN_BOTTOM_NOSLIP:
            discretization_->u(i, j) = 2 * settings_.NoSlipVelBottom[0] - discretization_->u(i, j + 1);
            discretization_->v(i, j) = settings_.NoSlipVelBottom[1];
            discretization_->g(i, j) = discretization_->v(i, j);
            break;
        case Mask::DOMAIN_LEFT_PRESSURE:
            discretization_->u(i, j) = discretization_->u(i+1, j);
            discretization_->f(i, j) = discretization_->u(i, j);
            discretization_->v(i, j) = discretization_->v(i+1, j);
            discretization_->g(i, j) = discretization_->v(i, j);
            break;
        case Mask::DOMAIN_TOP_PRESSURE:
            discretization_->u(i, j) = discretization_->u(i, j-1);
            discretization_->f(i, j) = discretization_->u(i, j);
            discretization_->v(i, j) = discretization_->v(i, j-1);
            discretization_->g(i, j) = discretization_->v(i, j);
            break;
        case Mask::DOMAIN_RIGHT_PRESSURE:
            discretization_->u(i, j) = discretization_->u(i-1, j);
            discretization_->f(i, j) = discretization_->u(i, j);
            discretization_->v(i, j) = discretization_->v(i-1, j);
            discretization_->g(i, j) = discretization_->v(i, j);
            break;
        case Mask::DOMAIN_BOTTOM_PRESSURE:
            discretization_->u(i, j) = discretization_->u(i, j+1);
            discretization_->f(i, j) = discretization_->u(i, j);
            discretization_->v(i, j) = discretization_->v(i, j+1);
            discretization_->g(i, j) = discretization_->v(i, j);
            break;
        }
    }
}

void Boundary::setVelocityObstacleBC()
{
    for (int idx : obstacleBoundaryCells_)
    {
        int i = idx % mask_->size()[0];
        int j = idx / mask_->size()[0];
        switch ((*mask_)(i, j))
        {
        case Mask::OBSTACLE_BORDER_LEFT:
            discretization_->v(i, j) = -discretization_->v(i - 1, j);
            discretization_->u(i - 1, j) = 0.0;
            discretization_->f(i - 1, j) = discretization_->u(i - 1, j);
            break;
        case Mask::OBSTACLE_BORDER_TOP:
            discretization_->u(i, j) = -discretization_->u(i, j + 1);
            discretization_->v(i, j) = 0.0;
            discretization_->g(i, j) = discretization_->v(i, j);
            break;
        case Mask::OBSTACLE_CORNER_TOP_LEFT:
            discretization_->u(i - 1, j) = 0.0;
            discretization_->f(i - 1, j) = discretization_->u(i - 1, j);
            discretization_->v(i, j) = 0.0;
            discretization_->g(i, j) = discretization_->v(i, j);
            discretization_->u(i, j) = -discretization_->u(i, j + 1);
            discretization_->v(i, j - 1) = -discretization_->v(i - 1, j - 1);
            break;
        case Mask::OBSTACLE_BORDER_RIGHT:
            discretization_->v(i, j) = -discretization_->v(i + 1, j);
            discretization_->u(i, j) = 0.0;
            discretization_->f(i, j) = discretization_->u(i, j);
            break;
        case Mask::OBSTACLE_CORNER_TOP_RIGHT:
            discretization_->u(i, j) = 0.0;
            discretization_->f(i, j) = discretization_->u(i, j);
            discretization_->v(i, j) = 0.0;
            discretization_->g(i, j) = discretization_->v(i, j);
            discretization_->u(i - 1, j) = -discretization_->u(i - 1, j + 1);
            discretization_->v(i, j - 1) = -discretization_->v(i + 1, j - 1);
            break;
        case Mask::OBSTACLE_BORDER_BOTTOM:
            discretization_->u(i, j) = -discretization_->u(i, j - 1);
            discretization_->v(i, j - 1) = 0.0;
            discretization_->g(i, j - 1) = 0.0;
            break;
        case Mask::OBSTACLE_CORNER_BOTTOM_RIGHT:
            discretization_->u(i, j) = 0.0;
            discretization_->f(i, j) = discretization_->u(i, j);
            discretization_->v(i, j - 1) = 0.0;
            discretization_->g(i, j - 1) = discretization_->v(i, j - 1);
            discretization_->u(i - 1, j) = -discretization_->u(i - 1, j - 1);
            discretization_->v(i, j) = -discretization_->v(i + 1, j);
            break;
        case Mask::OBSTACLE_CORNER_BOTTOM_LEFT:
            discretization_->u(i - 1, j) = 0.0;
            discretization_->f(i - 1, j) = discretization_->u(i, j);
            discretization_->v(i, j - 1) = 0.0;
            discretization_->g(i, j - 1) = discretization_->v(i, j - 1);
            discretization_->u(i, j) = -discretization_->u(i, j - 1);
            discretization_->v(i, j) = -discretization_->v(i - 1, j);
            break;
        }
    }
}

void Boundary::setVelocitySurfaceBC(){

    double dxByDy = discretization_->dx() / discretization_->dy();
    double dyByDx = discretization_->dy() / discretization_->dx();
    // Should the dt update step happen here? Then per loop only one execution is allowed
    double dt = 1;
    
    for (int idx : fluidBoundaryCells_)
    {
        int i = idx % mask_->size()[0];
        int j = idx / mask_->size()[0];
        
    // should be solved in the following order:
    //     from left to right (increasing i)
    //     from bottom to top (increasing j)
    

    switch ((*mask_)(i, j)) {
        case Mask::FLUID_BORDER_LEFT:
            // u - CONTINUITY
            discretization_->u(i - 1, j) = discretization_->u(i, j) 
                + dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));

            // v - TANGENTIAL
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1)
                    + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));
            }
            break;

        case Mask::FLUID_BORDER_TOP:
            // v - CONTINUITY
            discretization_->v(i, j) = discretization_->v(i, j - 1)
                + dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));

            // u - TANGENTIAL
            if (mask_->isAir(i - 1, j + 1)){
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j)
                    - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
            }
            break;
        case Mask::FLUID_BORDER_RIGHT:
            // u - CONTINUITY
            discretization_->u(i, j) = discretization_->u(i - 1, j)
                - dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));

            // v - TANGENTIAL
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1)
                    + dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
            }
            break;
        case Mask::FLUID_BORDER_BOTTOM:
            // v - CONTINUITY
            discretization_->v(i, j - 1) = discretization_->v(i, j)
                + dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));
            
            // u - TANGENTIAL
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j)
                    + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
            }
            break;
        case Mask::FLUID_CORNER_TOP_LEFT:
            // u & v - CONTINUITY (CHAIN)
            discretization_->u(i - 1, j) = discretization_->u(i, j);
            discretization_->v(i, j) = discretization_->v(i, j - 1);
            if (mask_->isAir(i - 1, j + 1)) {
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j); // chain, u(i, j) is equvivalent
                discretization_->v(i - 1, j) = discretization_->v(i, j);
            }

            // v - TANGENTIAL
            if (mask_->isAir(i - 1, j - 1)) {
            discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1)
                + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));

            }
            break;
        case Mask::FLUID_CORNER_TOP_RIGHT:
            // u & v - CONTINUITY (CHAIN)
            discretization_->u(i, j) = discretization_->u(i - 1, j);
            discretization_->v(i, j) = discretization_->v(i, j - 1);
            if (mask_->isAir(i + 1, j + 1)){
                discretization_->u(i, j + 1) = discretization_->u(i, j);
                discretization_->v(i + 1, j) = discretization_->v(i, j);
            }

            // u & v - TANGENTIAL
            if (mask_->isAir(i - 1, j + 1)) {
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j)
                    - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
            }
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1)
                    - dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
            }
            break;
        case Mask::FLUID_CORNER_BOTTOM_RIGHT:
            // u & v - CONTINUITY (CHAIN)
            discretization_->u(i, j) = discretization_->u(i - 1, j);
            discretization_->v(i, j - 1) = discretization_->v(i, j);
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1);
                discretization_->u(i, j - 1) = discretization_->u(i, j);
            } 

            // u - TANGENTIAL
            if (mask_->isAir(i - 1, j - 1)) {
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j)
                    + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
            }
            break;
        case Mask::FLUID_CORNER_BOTTOM_LEFT:
            // u & v - CONTINUITY (CHAIN)
            discretization_->u(i - 1, j) = discretization_->u(i, j);
            discretization_->v(i, j - 1) = discretization_->v(i, j);
            if (mask_->isAir(i - 1, j - 1))
            {
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j);
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1);
            } 
            break;
        case Mask::FLUID_COLUMN_HORIZONTAL:
            // // v
            // discretization_->v(i, j) = discretization_->v(i, j) + dt * settings_.g[1];
            // discretization_->v(i, j - 1) = discretization_->v(i, j - 1) + dt * settings_.g[1];
            // // u
            // if (mask_->isAir(i - 1, j + 1)){
            //     discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j) 
            //         - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
            // }
            // if (mask_->isAir(i - 1, j - 1)){
            //     discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j) 
            //         + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
            // }
            break;
        case Mask::FLUID_COLUMN_VERTICAL:
            // // u
            // discretization_->u(i, j) = discretization_->u(i, j) + dt * settings_.g[0];
            // discretization_->u(i - 1, j) = discretization_->u(i - 1, j) + dt * settings_.g[0];

            // if (mask_->isAir(i - 1, j - 1)){
            //     discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1) 
            //         + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));
            // }
            // if (mask_->isAir(i + 1, j - 1)){
            //     discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1) 
            //         - dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
            // }
            break;
        case Mask::FLUID_SINGLE_LEFT:
        //     // v
        //     discretization_->v(i, j) = discretization_->v(i, j) + dt * settings_.g[1];
        //     discretization_->v(i, j - 1) = discretization_->v(i, j - 1) + dt * settings_.g[1];
        //     // u
        //     discretization_->u(i - 1, j) = discretization_->u(i, j)
        //         + dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));
        //     // others
        //     if (mask_->isAir(i - 1, j + 1)){
        //         discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j);
        //         discretization_->v(i - 1, j) = discretization_->v(i, j);
        //     }
        //     if (mask_->isAir(i - 1, j - 1)){
        //         discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j);
        //         discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1);
        //     }
            break;
        case Mask::FLUID_SINGLE_TOP:
        //     // u
        //     discretization_->u(i, j) = discretization_->u(i, j) + dt * settings_.g[0];
        //     discretization_->u(i - 1, j) = discretization_->u(i - 1, j) + dt * settings_.g[0];
        //     // v
        //     discretization_->v(i, j) = discretization_->v(i, j - 1)
        //         - dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));
        //     // others
        //     if (mask_->isAir(i - 1, j + 1)){
        //         discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j);
        //         discretization_->v(i - 1, j) = discretization_->v(i, j);
        //     }
        //     if (mask_->isAir(i + 1, j + 1)){
        //         discretization_->u(i, j + 1) = discretization_->u(i, j);
        //         discretization_->v(i + 1, j) = discretization_->v(i, j);
        //     }

        //     // Tangential stress condition
        //     if (mask_->isAir(i - 1, j - 1)){
        //         discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1)
        //             + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));
        //     }
        //     if (mask_->isAir(i + 1, j - 1)){
        //         discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1)
        //             - dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
        //     }
            break;
        case Mask::FLUID_SINGLE_RIGHT:
        //     // v
        //     discretization_->v(i, j) = discretization_->v(i, j) + dt * settings_.g[1];
        //     discretization_->v(i, j - 1) = discretization_->v(i, j - 1) + dt * settings_.g[1];

        //     // u
        //     discretization_->u(i, j) = discretization_->u(i - 1, j)
        //         - dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));
        //     // others
        //     if (mask_->isAir(i + 1, j + 1)){
        //         discretization_->u(i, j + 1) = discretization_->u(i, j);
        //         discretization_->v(i + 1, j) = discretization_->v(i, j);
        //     }
        //     if (mask_->isAir(i + 1, j - 1)){
        //         discretization_->u(i + 1, j - 1) = discretization_->u(i, j);
        //         discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1);
        //     }

        //     // Tangential stress condition
        //     if (mask_->isAir(i - 1, j - 1)){
        //         discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j)
        //             + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
        //     }
        //     if (mask_->isAir(i - 1, j + 1)){
        //         discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j)
        //             - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
        //     }

            break;
        case Mask::FLUID_SINGLE_BOTTOM:
        //     // u
        //     discretization_->u(i, j) = discretization_->u(i, j) + dt * settings_.g[0];
        //     discretization_->u(i - 1, j) = discretization_->u(i - 1, j) + dt * settings_.g[0];
        //     // v
        //     discretization_->v(i, j - 1) = discretization_->v(i, j)
        //         + dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));
        //     // others
        //     if (mask_->isAir(i - 1, j - 1)){
        //         discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j);
        //         discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1);
        //     }
        //     if (mask_->isAir(i + 1, j - 1)){
        //         discretization_->u(i, j - 1) = discretization_->u(i, j);
        //         discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1);
        //     }
            break;
        case Mask::FLUID_DROPLET:
        //     // u
        //     discretization_->u(i, j) = discretization_->u(i, j) + dt * settings_.g[0];
        //     discretization_->u(i - 1, j) = discretization_->u(i - 1, j) + dt * settings_.g[0];
        //     // v
        //     discretization_->v(i, j) = discretization_->v(i, j) + dt * settings_.g[1];
        //     discretization_->v(i, j - 1) = discretization_->v(i, j - 1) + dt * settings_.g[1];

        //     // others
        //     if (mask_->isAir(i - 1, j - 1)){
        //         discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j);
        //         discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1);
        //     }
        //     if (mask_->isAir(i - 1, j + 1)){
        //         discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j);
        //         discretization_->v(i - 1, j) = discretization_->v(i, j);
        //     }
        //     if (mask_->isAir(i + 1, j + 1)){
        //         discretization_->u(i, j + 1) = discretization_->u(i, j);
        //         discretization_->v(i + 1, j) = discretization_->v(i, j);
        //     }
        //     if (mask_->isAir(i + 1, j - 1)){
        //         discretization_->u(i, j - 1) = discretization_->u(i, j);
        //         discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1);
        //     }
            break;
        default:
            std::cout << "ERROR" << (*mask_)(i, j) << "i" << i << " j " << j << std::endl;
    }
    }
}



void Boundary::setVelocitySurfaceBC(double dt){

    double dxByDy = discretization_->dx() / discretization_->dy();
    double dyByDx = discretization_->dy() / discretization_->dx();
    // Should the dt update step happen here? Then per loop only one execution is allowed
    
    for (int idx : fluidBoundaryCells_)
    {
        int i = idx % mask_->size()[0];
        int j = idx / mask_->size()[0];
        
    // should be solved in the following order:
    //     from left to right (increasing i)
    //     from bottom to top (increasing j)
    

    switch ((*mask_)(i, j)) {
        case Mask::FLUID_BORDER_LEFT:
            // u - CONTINUITY
            discretization_->u(i - 1, j) = discretization_->u(i, j) 
                + dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));

            // v - TANGENTIAL
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1)
                    + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));
            }
            break;

        case Mask::FLUID_BORDER_TOP:
            // v - CONTINUITY
            discretization_->v(i, j) = discretization_->v(i, j - 1)
                + dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));

            // u - TANGENTIAL
            if (mask_->isAir(i - 1, j + 1)){
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j)
                    - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
            }
            break;
        case Mask::FLUID_BORDER_RIGHT:
            // u - CONTINUITY
            discretization_->u(i, j) = discretization_->u(i - 1, j)
                - dxByDy * (discretization_->v(i, j) - discretization_->v(i, j - 1));

            // v - TANGENTIAL
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1)
                    + dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
            }
            break;
        case Mask::FLUID_BORDER_BOTTOM:
            // v - CONTINUITY
            discretization_->v(i, j - 1) = discretization_->v(i, j)
                + dyByDx * (discretization_->u(i, j) - discretization_->u(i - 1, j));
            
            // u - TANGENTIAL
            if (mask_->isAir(i - 1, j - 1)){
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j)
                    + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
            }
            break;
        case Mask::FLUID_CORNER_TOP_LEFT:
            // u & v - CONTINUITY (CHAIN)
            discretization_->u(i - 1, j) = discretization_->u(i, j);
            discretization_->v(i, j) = discretization_->v(i, j - 1);
            if (mask_->isAir(i - 1, j + 1)) {
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j); // chain, u(i, j) is equvivalent
                discretization_->v(i - 1, j) = discretization_->v(i, j);
            }

            // v - TANGENTIAL
            if (mask_->isAir(i - 1, j - 1)) {
            discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1)
                + dxByDy * (discretization_->u(i - 1, j) - discretization_->u(i - 1, j - 1));

            }
            break;
        case Mask::FLUID_CORNER_TOP_RIGHT:
            // u & v - CONTINUITY (CHAIN)
            discretization_->u(i, j) = discretization_->u(i - 1, j);
            discretization_->v(i, j) = discretization_->v(i, j - 1);
            if (mask_->isAir(i + 1, j + 1)){
                discretization_->u(i, j + 1) = discretization_->u(i, j);
                discretization_->v(i + 1, j) = discretization_->v(i, j);
            }

            // u & v - TANGENTIAL
            if (mask_->isAir(i - 1, j + 1)) {
                discretization_->u(i - 1, j + 1) = discretization_->u(i - 1, j)
                    - dyByDx * (discretization_->v(i, j) - discretization_->v(i - 1, j));
            }
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1)
                    - dxByDy * (discretization_->u(i, j) - discretization_->u(i, j - 1));
            }
            break;
        case Mask::FLUID_CORNER_BOTTOM_RIGHT:
            // u & v - CONTINUITY (CHAIN)
            discretization_->u(i, j) = discretization_->u(i - 1, j);
            discretization_->v(i, j - 1) = discretization_->v(i, j);
            if (mask_->isAir(i + 1, j - 1)){
                discretization_->v(i + 1, j - 1) = discretization_->v(i, j - 1);
                discretization_->u(i, j - 1) = discretization_->u(i, j);
            } 

            // u - TANGENTIAL
            if (mask_->isAir(i - 1, j - 1)) {
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j)
                    + dyByDx * (discretization_->v(i, j - 1) - discretization_->v(i - 1, j - 1));
            }
            break;
        case Mask::FLUID_CORNER_BOTTOM_LEFT:
            // u & v - CONTINUITY (CHAIN)
            discretization_->u(i - 1, j) = discretization_->u(i, j);
            discretization_->v(i, j - 1) = discretization_->v(i, j);
            if (mask_->isAir(i - 1, j - 1))
            {
                discretization_->u(i - 1, j - 1) = discretization_->u(i - 1, j);
                discretization_->v(i - 1, j - 1) = discretization_->v(i, j - 1);
            } 
            break;
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
            break;
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
            std::cout << "ERROR" << std::endl;
            break;
    }
    

    }
}








bool Boundary::doCalculateF(int i, int j) const {
    if (mask_->isFluid(i, j) && mask_->isFluid(i + 1, j)){
        return true;
    } else {
        return false;
    }
}

bool Boundary::doCalculateG(int i, int j) const {
    if (mask_->isFluid(i, j) && mask_->isFluid(i, j + 1)){
        return true;
    } else {
        return false;
    }
}

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
            if (mask_->isDomainBoundary(i, j))
                DomainBoundaryCells_.push_back(i + j * mask_->size()[0]);
            if (mask_->isObstacleBoundary(i, j))
                ObstacleBoundaryCells_.push_back(i + j * mask_->size()[0]);
        }
    }
}

void Boundary::setPressureBoundaryValues()
{
    setPressureDomainBC();
    setPressureObstacleBC();
}

void Boundary::setPressureDomainBC()
{
    for (int idx : DomainBoundaryCells_)
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
    for (int idx : ObstacleBoundaryCells_)
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

void Boundary::setVelocityBoundaryValues()
{
    setVelocityDomainBC();
    setVelocityObstacleBC();
}

void Boundary::setVelocityDomainBC()
{
    for (int idx : DomainBoundaryCells_)
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
    for (int idx : ObstacleBoundaryCells_)
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
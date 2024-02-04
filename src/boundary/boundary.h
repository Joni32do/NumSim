#pragma once

#include "mask.h"
#include "../discretization/discretization.h"
#include "../settings_parser/settings.h"
#include <vector>
#include <memory>

class Boundary
{
    public:
        Boundary(std::shared_ptr<Mask> mask, std::shared_ptr<Discretization> discretization, Settings settings);
        void setPressureBoundaryValues();
        void setVelocityBoundaryValues();
        std::shared_ptr<Mask> mask_;

    private:
        void createBoundaryCellsLists();

        // Set Pressure
        void setPressureDomainBC();
        void setPressureObstacleBC();
        void setPressureSurfaceBC();


        // Set Velocity
        void setVelocityDomainBC();
        void setVelocityObstacleBC();


        std::shared_ptr<Discretization> discretization_;
        Settings settings_;

        std::vector<int> DomainBoundaryCells_;
        std::vector<int> ObstacleBoundaryCells_;
        std::vector<int> FluidBoundaryCells_;

};
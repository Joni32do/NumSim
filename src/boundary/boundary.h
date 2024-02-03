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
        void createBoundaryCellsLists();
        std::shared_ptr<Mask> mask_;


    private:
        void setPressureDomainBC();
        void setPressureObstacleBC();

        void setVelocityDomainBC();
        void setVelocityObstacleBC();

        std::shared_ptr<Discretization> discretization_;
        Settings settings_;

        std::vector<int> boundaryCells_;

        std::vector<int> DomainBoundaryCells_;
        std::vector<int> ObstacleBoundaryCells_;
};
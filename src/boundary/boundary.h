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

        int getNumberOfBoundaryCells() const;

    private:
        void setPressureBoundaryObstacle(int i, int j);
        void setPressureBoundarySurface(int i, int j);

        // Helpers to set Boundary values
        void setVelocityBoundaryObstacleU(int i, int j);
        void setVelocityBoundaryObstacleV(int i, int j);

        void setVelocityBoundarySurfaceU(int i, int j);
        void setVelocityBoundarySurfaceV(int i, int j);

        std::shared_ptr<Discretization> discretization_;
        std::shared_ptr<Mask> mask_;
        Settings settings_;

        std::vector<int> boundaryCells_;

        std::vector<int> DomainBoundaryCells_;
        std::vector<int> ObstacleBoundaryCells_;

        void setPressureDomainBC();
        void setPressureObstacleBC();

        void setVelocityDomainBC();
        void setVelocityObstacleBC();
};
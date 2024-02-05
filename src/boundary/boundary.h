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
        void setVelocityBoundaryValues(double dt);
        std::shared_ptr<Mask> mask_;


        bool doCalculateF(int i, int j) const;
        bool doCalculateG(int i, int j) const;
        void updateBoundary();

    private:
        void createBoundaryCellsLists();

        // Set Pressure
        void setPressureDomainBC();
        void setPressureObstacleBC();
        void setPressureSurfaceBC();


        // Set Velocity
        void setVelocityDomainBC();
        void setVelocityObstacleBC();
        void setVelocitySurfaceBC();
        void setVelocitySurfaceBC(double dt);


        std::shared_ptr<Discretization> discretization_;
        Settings settings_;

        std::vector<int> domainBoundaryCells_;
        std::vector<int> obstacleBoundaryCells_;
        std::vector<int> fluidBoundaryCells_;

};
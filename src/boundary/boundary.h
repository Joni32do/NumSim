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

        int getNumberOfBoundaryCells() const;

    private:
        void setPressureBoundaryObstacle(int i, int j);
        void setPressureBoundarySurface(int i, int j);

        // Helpers to set Boundary values
        void setVelocityBoundaryObstacleU(int i, int j);
        void setVelocityBoundaryObstacleV(int i, int j);

        void setVelocityBoundarySurface(int i, int j);

        std::shared_ptr<Discretization> discretization_;
        std::shared_ptr<Mask> mask_;
        Settings settings_;

        std::vector<int> boundaryCells_;

};
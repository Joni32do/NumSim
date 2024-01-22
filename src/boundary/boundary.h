#pragma once

#include "boundary/mask.h"
#include "discretization/discretization.h"
#include "settings_parser/settings.h"
#include <vector>
#include <memory>

class Boundary
{
    public:
        Boundary(std::shared_ptr<Mask> mask, std::shared_ptr<Discretization> discretization, Settings settings);
        void setPressureBoundaryValues();
        void setVelocityBoundaryValues();

    private:
        void setPressureBoundarySurface(int i, int j);
        void setPressureBoundaryObstacle(int i, int j);

        void setVelocityBoundarySurface(int i, int j);
        void setVelocityBoundaryObstacle(int i, int j);

        void setUBoundaryValues();
        void setVBoundaryValues();

        std::shared_ptr<Discretization> discretization_;
        std::shared_ptr<Mask> mask_;
        Settings settings_;

        std::vector<int> boundaryCells_;

};
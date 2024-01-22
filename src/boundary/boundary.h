#pragma once

#include "boundary/mask.h"
#include "discretization/discretization.h"
#include <vector>
#include <memory>

class Boundary
{
    public:
        Boundary(std::shared_ptr<Mask> mask, std::shared_ptr<Discretization> discretization);
        void setPressureBoundaryValues();

    private:
        void setPressureBoundarySurface(int i, int j);
        void setPressureBoundaryObstacle(int i, int j);

        std::shared_ptr<Discretization> discretization_;
        std::shared_ptr<Mask> mask_;
        std::vector<int> boundaryCells_;

};
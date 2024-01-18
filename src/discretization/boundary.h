#pragma once

#include "storage/mask.h"

class Boundary
{
    public:
        void updateBoundaryPressure();

    private:
        void updateBoundaryPressureDomain();
        void updateBoundaryPressureObstacle();
        void updateBoundaryPressureSurfaceTension();

};
#pragma once

#include <iostream>
#include <memory>

#include "discretization/discretization.h"


class FluidTracer {
    public:
        FluidTracer(int numParticlesperCell, std::shared_ptr<Discretization> discretization);

        void updatePosition(double dt);

        void updateMask();



    private:
        int numParticles_;
        int numParticlesperCell_;
        std::shared_ptr<Discretization> discretization_;
        std::vector<double> x_;
        std::vector<double> y_;
};
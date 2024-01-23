#pragma once

#include <iostream>
#include <memory>

#include "../discretization/discretization.h"
#include "../boundary/boundary.h"


class FluidTracer {
    public:
        FluidTracer(int numParticlesperCell,
                    std::shared_ptr<Discretization> discretization,
                    std::shared_ptr<Mask> mask);

        void updatePosition(double dt);

        void updateMask();

        int getNumberOfParticles() const;



    private:

        void initializeFluidCell(int i, int j, int numParticlesPerCell);

        int numParticles_;
        int numParticlesPerCell_; // <! is usually because of ceil larger then numParticlesPerCell> 
        double seedRelationDyDx_;
        int n_x;
        int n_y;

        std::shared_ptr<Discretization> discretization_;
        std::shared_ptr<Mask> mask_;
        std::vector<double> x_;
        std::vector<double> y_;
};
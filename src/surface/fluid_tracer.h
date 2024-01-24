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

        /**
         * @brief Update the position of the particles according to the velocity field
         * 
         * Uses the ptr to the discretization to get the velocity field $u$ and $v$
         * 
         * Checks for collision with border or obstacle and reflects the particle at this position
         * 
         * @param dt 
         */
        void updatePosition(double dt);

        /**
         * @brief Update after the particles move there are new Air or Fluid cells
         * 
         * @requires that afterwards the mask borders are updated
         * 
         */
        void updateMask();


        /**
         * @brief Get the Number Of Particles object
         * 
         * @return int 
         */
        int getNumberOfParticles() const;


        /**
         * @brief Get the Particle Position, index starts at 0
         * 
         * @param i 
         * @return std::array<double, 2> 
         */
        std::array<double, 2> getParticlePosition(int i) const;

        int val2CellX(double xVal);
        int val2CellY(double yVal);

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
#pragma once

#include <iostream>
#include <memory>
#include <cmath>
#include <cassert>

#include "../discretization/discretization.h"
#include "../boundary/boundary.h"


class FluidTracer {
    public:
        FluidTracer(int numParticlesperCell,
                    std::shared_ptr<Discretization> discretization,
                    std::shared_ptr<Mask> mask);

        // Mainly for testing purposes
        FluidTracer(std::vector<double> x, std::vector<double> y,
                    std::shared_ptr<Discretization> discretization,
                    std::shared_ptr<Mask> mask);

        /**
         * @brief Update the position of the particles according to the velocity field and sets mask accordingly
         * 
         * get the velocity field $u$ and $v$
         * Checks for collision with border or obstacle and reflects the particle at this position
         * Update the mask where there are new Air or Fluid cells
         * 
         * @param dt 
         */
        void moveParticles(double dt);

        /**
         * @brief can be used to create a source cell for particles
         * 
         * fills the cell homogenously with particles by cell
         * 
         */
        void createParticles(double positionSourceX, double positionSourceY);

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
        /**
         * @brief initializes a single cell according to nx, ny and numParticlesPerCell_
         * 
         * @param i index of inner cells in x direction
         * @param j index of inner cells in y direction
         * @param idx current number of particles
         */
        void initializeFluidCell(int i, int j, int idx);

        void updateParticle(int i, double dt, double vel_x, double vel_y);


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
#include "fluid_tracer.h"

FluidTracer::FluidTracer(int numParticlesPerCell,
                         std::shared_ptr<Discretization> discretization,
                         std::shared_ptr<Mask> mask) {
    
    discretization_ = discretization;
    mask_ = mask;


    seedRelationDyDx_ = discretization_->dy()/ discretization_->dx();
    n_x = static_cast<int>(std::ceil(std::sqrt(numParticlesPerCell/seedRelationDyDx_)));
    n_y = static_cast<int>(std::ceil(n_x * seedRelationDyDx_));
    numParticlesPerCell_ = n_x * n_y;
    numParticles_ = numParticlesPerCell_ * mask_->getNumberOfFluidCells();
    // std::cout << "Actual particles per cell: " << numParticlesPerCell_ << std::endl;
    // std::cout << "Number of particles: " << numParticlesPerCell << std::endl;
    // std::cout << "nx: " << n_x << std::endl;
    // std::cout << "ny: " << n_y << std::endl;
    // std::cout << "seedRelationDyDx_: " << seedRelationDyDx_ << std::endl;
    
    // Resize for speed up
    x_.resize(numParticles_);
    y_.resize(numParticles_);


    for (int i = 0; i < discretization->nCells()[0]; i++) {
        for (int j = 0; j < discretization->nCells()[1]; j++) {
            if (mask_->isFluid(i, j)) {
                initializeFluidCell(i, j, numParticlesPerCell);
            }
        }
    }
}

void FluidTracer::initializeFluidCell(int i, int j, int numParticlesPerCell) {
    int idx = 0;
    for (int k = 0; k < n_x; k++) {
        for (int l = 0; l < n_y; l++) {
            x_[idx] = (k + 0.5) * discretization_->dx() / n_x;
            y_[idx] = (l + 0.5) * discretization_->dy() / n_y;
            idx++;
        }
    }
}

int FluidTracer::getNumberOfParticles() const {
    return numParticles_;
}
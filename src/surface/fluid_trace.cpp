#include "fluid_tracer.h"

FluidTracer::FluidTracer(int numParticlesperCell, std::shared_ptr<Discretization> discretization) {
    numParticles_ = numParticlesperCell;
    discretization_ = discretization;

    int n_x = discretization_->nCells()[0];
    int n_y = discretization_->nCells()[1];
    std::array<double, 2> physicalSize_ = {discretization_->meshWidth()[0]*n_x,
                                           discretization_->meshWidth()[1]*n_y};
    int numParticles_ = numParticlesperCell * discretization_->nCells()[0] * discretization_->nCells()[1];
    
    
    // Resize for speed up
    x_.resize(numParticles_);
    y_.resize(numParticles_);


    // Initialize particles
    // TODO: Obsticle

    for (int j = 0; j < n_y; j++) {
        for (int i = 0; i < n_x; i++) {
            // Invariant: distance to border most be 1/2 meshWidth such that no discretization artefacts occur
            for (int k = 0; k < numParticlesperCell; k++) {
                x_[i*numParticlesperCell + j*numParticlesperCell + k] = 2;
            }
        }
    }
}
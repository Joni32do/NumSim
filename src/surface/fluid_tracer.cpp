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

void FluidTracer::updatePosition(double dt) {
    for (int i = 0; i < numParticles_; i++) {
        double vel_x = discretization_->u().interpolateAt(x_[i], y_[i]);
        double vel_y = discretization_->v().interpolateAt(x_[i], y_[i]);
        double step_x = dt * vel_x;
        double step_y = dt * vel_y;

        // Check if collision with obstacle or boundary
        // This is slow (because of CFL condition only cells next to boundary could do this check)
        int idx_x = val2CellX(x_[i] + step_x);
        int idx_y = val2CellY(y_[i] + step_y);
        if (mask_->isObstacle(idx_x, idx_y)) {
            // collision on x or y axis
            double leftBorder = discretization_->dx()*(idx_x - 1); // -1 because idx starts at 0 with outer cell -dx
            double topBorder = discretization_->dy()*(idx_y);
            double rightBorder = discretization_->dx()*(idx_x);
            double lowerBorder = discretization_->dy()*(idx_y - 1);

            // four cases for collision 
            if (step_x >= 0 && step_y >= 0) {
                // need to check for collision with left and bottom border
                double dist_x = x_[i] - leftBorder;
                double dt_x = dist_x/vel_x;
                double dist_y = y_[i] - lowerBorder;
                double dt_y = dist_y/vel_y;
                if (dt_x < dt_y) {
                    // collision with left border
                    // first     dt_x * vel_x       then    - (dt - dt_x) * vel_x
                    step_x = (2*dt_x - dt) * vel_x;
                } else {
                    // collision with bottom border
                    step_y = (2*dt_y - dt) * vel_y;
                }
            }
            else if (step_x >= 0 && step_y < 0) {
                // need to check for collision with left and top border
                double dist_x = x_[i] - leftBorder;
                double dt_x = dist_x/vel_x;
                double dist_y = topBorder - y_[i]; // double negative 
                double dt_y = dist_y/vel_y;
                if (dt_x < dt_y) {
                    // collision with left border
                    // first     dt_x * vel_x       then    - (dt - dt_x) * vel_x
                    step_x = (2*dt_x - dt) * vel_x;
                } else {
                    // collision with top border
                    step_y = (2*dt_y - dt) * vel_y;
                }
            }
            else if (step_x < 0 && step_y >= 0) {
                // need to check for collision with right and bottom border
                double dist_x = rightBorder - x_[i];
                double dt_x = dist_x/vel_x;
                double dist_y = y_[i] - lowerBorder;
                double dt_y = dist_y/vel_y;
                if (dt_x < dt_y) {
                    // collision with right border
                    step_x = (2*dt_x - dt) * vel_x;
                } else {
                    // collision with bottom border
                    step_y = (2*dt_y - dt) * vel_y;
                }
            }
            else if (step_x < 0 && step_y < 0) {
                // need to check for collision with right and top border
                double dist_x = rightBorder - x_[i];
                double dt_x = dist_x/vel_x;
                double dist_y = topBorder - y_[i];
                double dt_y = dist_y/vel_y;
                if (dt_x < dt_y) {
                    // collision with right border
                    // first     dt_x * vel_x       then    - (dt - dt_x) * vel_x
                    step_x = (2*dt_x - dt) * vel_x;
                } else {
                    // collision with top border
                    step_y = (2*dt_y - dt) * vel_y;
                }
            }
        }

        x_[i] += step_x;
        y_[i] += step_y;
    }
}

int FluidTracer::val2CellX(double xVal){
    return static_cast<int>(std::floor(xVal/discretization_->dx())) + 1;
}

int FluidTracer::val2CellY(double yVal){
    return static_cast<int>(std::floor(yVal/discretization_->dy())) + 1;
}

std::array<double, 2> FluidTracer::getParticlePosition(int i) const {
    return {x_[i], y_[i]};
}
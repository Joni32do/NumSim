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
    
    // Resize for speed up
    x_.resize(numParticles_);
    y_.resize(numParticles_);
    ;
    for (int i = 1; i < mask_->size()[0] - 1; i++) {
        for (int j = 1; j < mask_->size()[1] - 1; j++) {
            if (mask_->isFluid(i, j)) {
                initializeFluidCell(i, j, numParticlesPerCell);
            }
        }
    }
}

FluidTracer::FluidTracer(std::vector<double> x, std::vector<double> y,
                        std::shared_ptr<Discretization> discretization,
                        std::shared_ptr<Mask> mask) {
    x_ = x;
    y_ = y;
    discretization_ = discretization;
    mask_ = mask;
    numParticles_ = x_.size();
}


void FluidTracer::initializeFluidCell(int i, int j, int numParticlesPerCell) {
    int idx = 0;
    double cell_x = discretization_->dx() * (i - 1);
    double cell_y = discretization_->dy() * (j - 1);
    // fills in from bottom left to top right
    for (int l = 0; l < n_y; l++) {
        for (int k = 0; k < n_x; k++) {
            x_[idx] = cell_x + (0.5 + (k * discretization_->dx())) / n_x;
            y_[idx] = cell_y + (0.5 + (l * discretization_->dy())) / n_y;
            idx++;
        }
    }
}


int FluidTracer::getNumberOfParticles() const {
    return numParticles_;
}


void FluidTracer::moveParticles(double dt) {
    // Cells without particle are air or obstacle
    mask_->resetMask();

    for (int i = 0; i < numParticles_; i++) {

        double vel_x = discretization_->u().interpolateAt(x_[i], y_[i]);
        double vel_y = discretization_->v().interpolateAt(x_[i], y_[i]);
        updateParticle(i, dt, vel_x, vel_y);

        // Update mask
        int idx_x = val2CellX(x_[i]);
        int idx_y = val2CellY(y_[i]);
        (*mask_)(idx_x, idx_y) = Mask::FLUID;
    }

    mask_->updateMaskBoundaries();
}


void FluidTracer::updateParticle(int i, double dt, double vel_x, double vel_y) {

    double step_x = dt * vel_x;
    double step_y = dt * vel_y;


    // Check for Collision with obstacle
    int idxNextCellX = val2CellX(x_[i] + step_x);
    int idxNextCellY = val2CellY(y_[i] + step_y);

    if (mask_->isObstacle(idxNextCellX, idxNextCellY)) {
        int idx_x = val2CellX(x_[i]);
        int idx_y = val2CellY(y_[i]);
        
        // x - Border
        double leftBorder = discretization_->dx()*(idx_x - 1); // -1 because idx starts at 0 with outer cell -dx
        double rightBorder = discretization_->dx()*(idx_x);

        // y - Border
        double lowerBorder = discretization_->dy()*(idx_y - 1);
        double topBorder = discretization_->dy()*(idx_y);

        // distance and time to border
        double dist_x, dt_x, dist_y, dt_y;
        double dtBeforeCollision;

        double new_vel_x = vel_x;
        double new_vel_y = vel_y;


        // Note: ZERO_DIVISON is no problem, because then dt is pos_inf, which is always larger and therefore the other case is taken

        // Uses CFL condition --> accelerate?
        
        // TODO: CORNER: double equality rounding errors?



        // Summary:
        // 1. `dt_x`: Calculate time till collision with x-Border (depending on direction of `vel_x` )
        // 2. `dt_y`: Calculate time till collision with y-Border (depending on direction of `vel_y` )
        // 3. Check which collision of the two possible happens first 
        //
        //     --> assign `dtBeforeCollision`, `new_vel_x` and `new_vel_y` accordingly


        if (vel_x >= 0) {
            dist_x = rightBorder - x_[i];
            dt_x = dist_x/vel_x;

            if (vel_y >= 0) {
                // direction TOP_RIGHT
                dist_y = topBorder - y_[i];
                dt_y = dist_y/vel_y;
                
                if (dt_x >= dt_y) {
                    // collision TOP
                    if (mask_->isObstacle(idx_x, idx_y + 1)){
                        new_vel_y = -vel_y;
                    }
                    dtBeforeCollision = dt_y;
                }
                if (dt_x <= dt_y){
                    // collision RIGHT
                    if (mask_->isObstacle(idx_x, idx_y + 1)){
                        new_vel_x = -vel_x;
                    }
                    dtBeforeCollision = dt_x;
                }
            } else if (vel_y < 0){
                // direction BOTTOM_RIGHT
                dist_y = lowerBorder - y_[i];
                dt_y = dist_y/vel_y; // -- = +
                
                if (dt_x >= dt_y) {
                    // collision BOTTOM
                    if (mask_->isObstacle(idx_x, idx_y - 1)){
                        new_vel_y = -vel_y;
                    }
                    dtBeforeCollision = dt_y;
                }
                if (dt_x <= dt_y){
                    // collision RIGHT
                    if (mask_->isObstacle(idx_x, idx_y + 1)){
                        new_vel_x = -vel_x;
                    }
                    dtBeforeCollision = dt_x;
                }
            } 
        } else if (vel_x < 0){
            dist_x = leftBorder - x_[i];
            dt_x = dist_x/vel_x; // -- = +

            if (vel_y >= 0) {
                // direction TOP_LEFT
                dist_y = topBorder - y_[i];
                dt_y = dist_y/vel_y;
                
                if (dt_x >= dt_y) {
                    // collision TOP
                    if (mask_->isObstacle(idx_x, idx_y + 1)){
                        new_vel_y = -vel_y;
                    }
                    dtBeforeCollision = dt_y;
                }
                if (dt_x <= dt_y){
                    // collision LEFT
                    if (mask_->isObstacle(idx_x, idx_y - 1)){
                        new_vel_x = -vel_x;
                    }
                    dtBeforeCollision = dt_x;
                }
            } else if (vel_y < 0){
                // direction BOTTOM_LEFT
                dist_y = lowerBorder - y_[i];
                dt_y = dist_y/vel_y; // -- = +
                
                if (dt_x <= dt_y) {
                    // collision BOTTOM
                    if (mask_->isObstacle(idx_x, idx_y - 1)){
                        new_vel_y = -vel_y;
                    }
                    dtBeforeCollision = dt_x;
                }
                if (dt_x >= dt_y){
                    // collision LEFT
                    if (mask_->isObstacle(idx_x, idx_y - 1)){
                        new_vel_x = -vel_x;
                    }
                    dtBeforeCollision = dt_y;
                }
            }
        }
        x_[i] += dtBeforeCollision * vel_x;
        y_[i] += dtBeforeCollision * vel_y;

        // dtBeforeCollision is always smaller
        // the maximum depth of recursion is 1
        updateParticle(i, dt - dtBeforeCollision, new_vel_x, new_vel_y);
    } else {

        // std::cout << "x=" << x_[i] << "+" << dt << "*" << vel_x << " -> ";
        x_[i] += step_x;
        // std::cout << x_[i] << std::endl;

        // std::cout << "  y=" << y_[i] << "+" << dt << "*" << vel_y << " -> ";
        y_[i] += step_y;
        // std::cout << y_[i] << std::endl;
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
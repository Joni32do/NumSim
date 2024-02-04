#include "fluid_tracer.h"

FluidTracer::FluidTracer(std::vector<double> x, std::vector<double> y,
                        std::shared_ptr<Discretization> discretization,
                        std::shared_ptr<Mask> mask) {
    x_ = x;
    y_ = y;
    discretization_ = discretization;
    mask_ = mask;
    numParticles_ = x_.size();

    numParticlesPerCell_ = 1;
    n_x = 1;
    n_y = 1;
}


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
    
    int placedParticles = 0;
    for (int i = 1; i < mask_->size()[0] - 1; i++) {
        for (int j = 1; j < mask_->size()[1] - 1; j++) {
            if (mask_->isFluid(i, j)) {
                initializeFluidCell(i, j, placedParticles);
                placedParticles += numParticlesPerCell_;
            }
        }
    }
    assert(placedParticles == numParticles_);
}


void FluidTracer::initializeFluidCell(int i, int j, int idx) {
    double cell_x = discretization_->dx() * (i - 1);
    double cell_y = discretization_->dy() * (j - 1);
    // fills in from bottom left to top right
    for (int l = 0; l < n_y; l++) {
        for (int k = 0; k < n_x; k++) {
            x_[idx] = cell_x + ((0.5 + k) * discretization_->dx()) / n_x;
            y_[idx] = cell_y + ((0.5 + l) * discretization_->dy()) / n_y;
            
            idx++;
        }
    }
}


void FluidTracer::createParticles(double positionSourceX, double positionSourceY){
    int idx_x = val2CellX(positionSourceX);
    int idx_y = val2CellY(positionSourceY);
    double newNumParticles_ = numParticles_ + numParticlesPerCell_;
    x_.resize(newNumParticles_);
    y_.resize(newNumParticles_);

    initializeFluidCell(idx_x, idx_y, numParticles_);
    numParticles_ = newNumParticles_;
}


int FluidTracer::getNumberOfParticles() const {
    return numParticles_;
}

std::array<int, 2> FluidTracer::cellOfParticle(int i){
    return {val2CellX(x_[i]), val2CellY(y_[i])};
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


void FluidTracer::moveParticles(double dt) {
    // Cells without particle are air or obstacle
    mask_->resetMask();

    for (int i = 0; i < numParticles_; i++) {
        
        std::array<double, 2> vel = {discretization_->u().interpolateAt(x_[i], y_[i]),
                                     discretization_->v().interpolateAt(x_[i], y_[i])};
        std::array<int, 2> idx = cellOfParticle(i);
        std::array<int, 2> newIdx = updateParticle(i, idx, dt, vel);

        // Safety feature -> can be removed for speed up
        if (!mask_->isObstacle(newIdx[0], newIdx[1])){
            (*mask_)(newIdx[0], newIdx[1]) = Mask::FLUID;
        }
    }
    mask_->setFluidBC();
}




std::array<int, 2> FluidTracer::updateParticle(int i, std::array<int, 2> idx, double dt, std::array<double,2> vel) {
    
    double newPos_x = x_[i] + dt * vel[0];
    double newPos_y = y_[i] + dt * vel[1];

    // x - Border
    double leftBorder = discretization_->dx()*(idx[0] - 1); // -1 because idx starts at 0 with outer cell -dx
    double rightBorder = discretization_->dx()*idx[0];
    bool inCellX = leftBorder < newPos_x && newPos_x < rightBorder;

    // y - Border
    double lowerBorder = discretization_->dy()*(idx[1] - 1);
    double topBorder = discretization_->dy()*idx[1];
    bool inCellY = lowerBorder < newPos_y && newPos_y < topBorder;

    if (inCellX && inCellY) {
        x_[i] = newPos_x;
        y_[i] = newPos_y;
        return idx;
    
    } else {
        // distance and time to border
        double dist_x, dt_x, dist_y, dt_y;
        double dtBeforeCollision;

        std::array<int, 2> newIdx = idx;
        std::array<double, 2> newVel = vel;

        // if any collision    
        double eps = 1e-10;

        if (dt < eps){
            return newIdx;
        }

        // Checks DIRECTION then COLLISION
        // U:
        if (vel[0] >= 0) {
            dist_x = rightBorder - x_[i];
            dt_x = dist_x/vel[0];

            if (vel[1] >= 0) {
                // direction TOP_RIGHT
                dist_y = topBorder - y_[i];
                dt_y = dist_y/vel[1];

                if (dt_y < dt_x) {
                    // collision TOP
                    if (mask_->isObstacle(idx[0], idx[1] + 1)){
                        newVel[1] = -vel[1];
                    } else {
                        newIdx[1] += 1;
                    }
                    dtBeforeCollision = dt_y;
                } else {
                    // collision RIGHT
                    if (mask_->isObstacle(idx[0] + 1, idx[1])){ //TODO: change 
                        newVel[0] = -vel[0];
                    } else {
                        newIdx[0] += 1;
                    }
                    dtBeforeCollision = dt_x;
                }
            } else if (vel[1] < 0){
                // direction BOTTOM_RIGHT
                dist_y = lowerBorder - y_[i];
                dt_y = dist_y/vel[1]; // -- = +
                
                if (dt_y < dt_x) {
                    // collision BOTTOM
                    if (mask_->isObstacle(idx[0], idx[1] - 1)){
                        newVel[1] = -vel[1];
                    } else {
                        newIdx[1] -= 1;
                    }
                    dtBeforeCollision = dt_y;
                } else  {
                    // collision RIGHT
                    if (mask_->isObstacle(idx[0] + 1, idx[1])){   // TODO: change
                        newVel[0] = -vel[0];
                    } else {
                        newIdx[0] += 1;
                    }
                    dtBeforeCollision = dt_x;
                }
            } 
        // U
        } else if (vel[0] < 0){
            dist_x = leftBorder - x_[i];
            dt_x = dist_x/vel[0]; // -- = +

            if (vel[1] >= 0) {
                // direction TOP_LEFT
                dist_y = topBorder - y_[i];
                dt_y = dist_y/vel[1];

                if (dt_y < dt_x) {
                    // collision TOP
                    if (mask_->isObstacle(idx[0], idx[1] + 1)){
                        newVel[1] = -vel[1];
                    } else {
                        newIdx[1] += 1;
                    }
                    dtBeforeCollision = dt_y;
                } else {
                    // collision LEFT
                    if (mask_->isObstacle(idx[0] - 1, idx[1])){ // TODO: change
                        newVel[0] = -vel[0];
                    } else {
                        newIdx[0] -= 1;
                    }
                    dtBeforeCollision = dt_x;
                }
            } else if (vel[1] < 0){
                // direction BOTTOM_LEFT
                dist_y = lowerBorder - y_[i];
                dt_y = dist_y/vel[1]; // -- = +

                if (dt_y < dt_x) {
                    // collision BOTTOM
                    if (mask_->isObstacle(idx[0], idx[1] - 1)){
                        newVel[1] = -vel[1];
                    } else {
                        newIdx[1] -= 1;
                    }
                    dtBeforeCollision = dt_y;
                } else {
                    // collision LEFT
                    if (mask_->isObstacle(idx[0] - 1, idx[1])){ //TODO: change
                        newVel[0] = -vel[0];
                    } else {
                        newIdx[0] -= 1;
                    }
                    dtBeforeCollision = dt_x;
                }
            }
        }
        // Bugfix for Floating Point Error
        if (dt_x < eps || dt_y < eps){
            dtBeforeCollision = eps;
        }
        
        // Happens if it was outside of cell but flies into the cell
        if (dt < dtBeforeCollision) {
            x_[i] += dt * vel[0];
            y_[i] += dt * vel[1];
            return newIdx;
        }

        x_[i] += dtBeforeCollision * vel[0];
        y_[i] += dtBeforeCollision * vel[1];
        double newDt = dt - dtBeforeCollision;

        if (newDt < eps){
            return newIdx;
        }    
        return updateParticle(i, newIdx, newDt, newVel);
    }
}






//////////////////////////////////////////////////////

// ONLY FOR TESTING PURPOSES

std::array<double, 2> FluidTracer::moveParticles(double dt, std::array<double, 2> vel) {
    // Cells without particle are air or obstacle
    mask_->resetMask();

    for (int i = 0; i < numParticles_; i++) {
        std::array<int, 2> idx1 = cellOfParticle(i);
        std::cout << " x: " << std::fixed << std::setprecision(3) << x_[0] 
                  << " y: " << std::fixed << std::setprecision(3) << y_[0] << std::endl;
        std::cout << "v_x: " << vel[0] << " v_y: " << vel[1] << std::endl;
        vel = updateParticle(i, idx1, dt, vel, 0);

        // Update mask
        std::array<int, 2> idx = cellOfParticle(i);
        if (!mask_->isObstacle(idx[0], idx[1])){
            (*mask_)(idx[0], idx[1]) = Mask::FLUID;
        }
    }

    mask_->setFluidBC();
    return vel;
}




std::array<double, 2> FluidTracer::updateParticle(int i, std::array<int, 2> idx, double dt, std::array<double,2> vel, int depth) {
    // std::cout << "depth: " << depth << "time " << dt << std::endl;
    if (depth > 2){
        std::cout << "----------------------------------" << std::endl;
        return vel;
    }
    double newPos_x = x_[i] + dt * vel[0];
    double newPos_y = y_[i] + dt * vel[1];

    // x - Border
    double leftBorder = discretization_->dx()*(idx[0] - 1); // -1 because idx starts at 0 with outer cell -dx
    double rightBorder = discretization_->dx()*idx[0];
    bool inCellX = leftBorder < newPos_x && newPos_x < rightBorder;

    // y - Border
    double lowerBorder = discretization_->dy()*(idx[1] - 1);
    double topBorder = discretization_->dy()*idx[1];
    bool inCellY = lowerBorder < newPos_y && newPos_y < topBorder;

    if (inCellX && inCellY) {
        x_[i] = newPos_x;
        y_[i] = newPos_y;
        return vel;
    
    } else {
        // distance and time to border
        double dist_x, dt_x, dist_y, dt_y;
        double dtBeforeCollision;

        std::array<int, 2> newIdx = idx;
        std::array<double, 2> newVel = vel;

        // if any collision    
        double eps = 1e-10;

        if (dt < eps){
            return vel;
        }

        // Note: ZERO_VELOCITY is no problem, because then dt is pos_inf, which is always larger

        // Checks DIRECTION then COLLISION
        // U:
        if (vel[0] >= 0) {
            dist_x = rightBorder - x_[i];
            dt_x = dist_x/vel[0];

            if (vel[1] >= 0) {
                // direction TOP_RIGHT
                dist_y = topBorder - y_[i];
                dt_y = dist_y/vel[1];

                if (dt_y < dt_x) {
                    // collision TOP
                    if (mask_->isObstacle(idx[0], idx[1] + 1)){
                        newVel[1] = -vel[1];
                    } else {
                        newIdx[1] += 1;
                    }
                    dtBeforeCollision = dt_y;
                } else {
                    // collision RIGHT
                    if (mask_->isObstacle(idx[0] + 1, idx[1])){ //TODO: change 
                        newVel[0] = -vel[0];
                    } else {
                        newIdx[0] += 1;
                    }
                    dtBeforeCollision = dt_x;
                }
            } else if (vel[1] < 0){
                // direction BOTTOM_RIGHT
                dist_y = lowerBorder - y_[i];
                dt_y = dist_y/vel[1]; // -- = +
                
                if (dt_y < dt_x) {
                    // collision BOTTOM
                    if (mask_->isObstacle(idx[0], idx[1] - 1)){
                        newVel[1] = -vel[1];
                    } else {
                        newIdx[1] -= 1;
                    }
                    dtBeforeCollision = dt_y;
                } else  {
                    // collision RIGHT
                    if (mask_->isObstacle(idx[0] + 1, idx[1])){   // TODO: change
                        newVel[0] = -vel[0];
                    } else {
                        newIdx[0] += 1;
                    }
                    dtBeforeCollision = dt_x;
                }
            } 
        // U
        } else if (vel[0] < 0){
            dist_x = leftBorder - x_[i];
            dt_x = dist_x/vel[0]; // -- = +

            if (vel[1] >= 0) {
                // direction TOP_LEFT
                dist_y = topBorder - y_[i];
                dt_y = dist_y/vel[1];

                if (dt_y < dt_x) {
                    // collision TOP
                    if (mask_->isObstacle(idx[0], idx[1] + 1)){
                        newVel[1] = -vel[1];
                    } else {
                        newIdx[1] += 1;
                    }
                    dtBeforeCollision = dt_y;
                } else {
                    // collision LEFT
                    if (mask_->isObstacle(idx[0] - 1, idx[1])){ // TODO: change
                        newVel[0] = -vel[0];
                    } else {
                        newIdx[0] -= 1;
                    }
                    dtBeforeCollision = dt_x;
                }
            } else if (vel[1] < 0){
                // direction BOTTOM_LEFT
                dist_y = lowerBorder - y_[i];
                dt_y = dist_y/vel[1]; // -- = +

                if (dt_y < dt_x) {
                    // collision BOTTOM
                    if (mask_->isObstacle(idx[0], idx[1] - 1)){
                        newVel[1] = -vel[1];
                    } else {
                        newIdx[1] -= 1;
                    }
                    dtBeforeCollision = dt_y;
                } else {
                    // collision LEFT
                    if (mask_->isObstacle(idx[0] - 1, idx[1])){ //TODO: change
                        newVel[0] = -vel[0];
                    } else {
                        newIdx[0] -= 1;
                    }
                    dtBeforeCollision = dt_x;
                }
            }
        }
        // Bugfix for Floating Point Error
        if (dt_x < eps || dt_y < eps){
            dtBeforeCollision = eps;
        }
        
        // Happens if it was outside of cell but flies into the cell
        if (dt < dtBeforeCollision) {
            x_[i] += dt * vel[0];
            y_[i] += dt * vel[1];
            return vel;
        }

        x_[i] += dtBeforeCollision * vel[0];
        y_[i] += dtBeforeCollision * vel[1];
        double newDt = dt - dtBeforeCollision;

        if (newDt < eps){
            return newVel;
        }    
        return updateParticle(i, newIdx, newDt, newVel, depth + 1);
    }
}
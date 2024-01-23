#include <gtest/gtest.h>
#include "../src/surface/fluid_tracer.h"
#include "../src/discretization/central_differences.h"

TEST(FluidTracer, InitializeSingleCell){
    std::array<int,2> n_cells = {1, 1};
    std::array<double,2> meshWidth = {1.0, 1.0};
    std::shared_ptr<Mask> mask = std::make_shared<Mask>(n_cells);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(n_cells, meshWidth);
    int numParticlesPerCell = 9;
    FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);
    EXPECT_GE(fluid_tracer.getNumberOfParticles(), numParticlesPerCell);

    int numParticlesPerCell2 = 100;
    FluidTracer fluid_tracer2(numParticlesPerCell2, discretization, mask);
    EXPECT_GE(fluid_tracer2.getNumberOfParticles(), numParticlesPerCell2);

    int numParticlesPerCell3 = 7;
    FluidTracer fluid_tracer3(numParticlesPerCell3, discretization, mask);
    EXPECT_GE(fluid_tracer3.getNumberOfParticles(), numParticlesPerCell3);
};


TEST(FluidTracer, val2CellXAndY){
    std::array<int,2> n_cells = {1, 1};
    std::array<double,2> meshWidth = {1.0, 1.0};
    std::shared_ptr<Mask> mask = std::make_shared<Mask>(n_cells);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(n_cells, meshWidth);
    int numParticlesPerCell = 9;
    FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);
    EXPECT_EQ(fluid_tracer.val2CellX(0.5), 1);
    EXPECT_EQ(fluid_tracer.val2CellX(0.0), 1);
    EXPECT_EQ(fluid_tracer.val2CellX(-0.5), 0);
    EXPECT_EQ(fluid_tracer.val2CellX(1.5), 2);

    EXPECT_EQ(fluid_tracer.val2CellY(0.5), 1);
    EXPECT_EQ(fluid_tracer.val2CellY(0.0), 1);
    EXPECT_EQ(fluid_tracer.val2CellY(-0.5), 0);
    EXPECT_EQ(fluid_tracer.val2CellY(1.5), 2);
}

TEST(FluidTracer, updatePositionWithoutBorderCollision){
    std::array<int,2> n_cells = {1, 1};
    std::array<double,2> meshWidth = {1.0, 1.0};
    std::shared_ptr<Mask> mask = std::make_shared<Mask>(n_cells);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(n_cells, meshWidth);
    double vel_x = 0.01;
    discretization->u(0,0) = vel_x;
    discretization->u(1,0) = vel_x;
    discretization->u(0,1) = vel_x;
    discretization->u(1,1) = vel_x;
    discretization->u(0,2) = vel_x;
    discretization->u(1,2) = vel_x;
    int numParticlesPerCell = 1;
    FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);
    EXPECT_EQ(fluid_tracer.getNumberOfParticles(), 1);
    double old_pos_x = fluid_tracer.getParticlePosition(0)[0];
    double dt = 0.1;
    fluid_tracer.updatePosition(dt);
    EXPECT_EQ(fluid_tracer.getParticlePosition(0)[0], old_pos_x + dt * vel_x);
}
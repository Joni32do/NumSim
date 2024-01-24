#include <gtest/gtest.h>
#include "../src/surface/fluid_tracer.h"
#include "../src/discretization/central_differences.h"

TEST(FluidTracer, NParticlesSingleCell){
    /*
    Test for a single cell (with 8 boundary cells) how many particles are placed
    */
    std::array<int,2> n_cells = {1, 1};
    std::array<double,2> meshWidth = {1.0, 1.0};
    std::shared_ptr<Mask> mask = std::make_shared<Mask>(n_cells);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(n_cells, meshWidth);
    int numParticlesPerCell = 1;
    FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);
    EXPECT_GE(fluid_tracer.getNumberOfParticles(), numParticlesPerCell);

    int numParticlesPerCell2 = 9;
    FluidTracer fluid_tracer2(numParticlesPerCell2, discretization, mask);
    EXPECT_GE(fluid_tracer2.getNumberOfParticles(), numParticlesPerCell2);

    int numParticlesPerCell3 = 17;
    FluidTracer fluid_tracer3(numParticlesPerCell3, discretization, mask);
    EXPECT_GE(fluid_tracer3.getNumberOfParticles(), numParticlesPerCell3);

    int numParticlesPerCell4 = 100;
    FluidTracer fluid_tracer4(numParticlesPerCell4, discretization, mask);
    EXPECT_GE(fluid_tracer4.getNumberOfParticles(), numParticlesPerCell4);
    EXPECT_GE(fluid_tracer4.getNumberOfParticles(), fluid_tracer3.getNumberOfParticles());
    EXPECT_LE(fluid_tracer4.getNumberOfParticles(), 2 * numParticlesPerCell4);
};

TEST(FluidTracer, DistributionSingleCell){
    /*
    Test for a single cell (with 8 boundary cells) how many particles are placed
    */
    std::array<int,2> n_cells = {1, 1};
    std::array<double,2> meshWidth = {1.0, 1.0};
    std::shared_ptr<Mask> mask = std::make_shared<Mask>(n_cells);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(n_cells, meshWidth);
    int numParticlesPerCell = 1;
    FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);
    EXPECT_NEAR(fluid_tracer.getParticlePosition(0)[0], 0.5, 0.001);
    EXPECT_NEAR(fluid_tracer.getParticlePosition(0)[1], 0.5, 0.001);

    int numParticlesPerCell2 = 9;
    FluidTracer fluid_tracer2(numParticlesPerCell2, discretization, mask);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(0)[0], 0.166667, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(0)[1], 0.166667, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(1)[0], 0.5, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(1)[1], 0.166667, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(4)[0], 0.5, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(4)[1], 0.5, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(8)[0], 0.833333, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(8)[1], 0.833333, 0.001);

    int numParticlesPerCell3 = 17;
    FluidTracer fluid_tracer3(numParticlesPerCell3, discretization, mask);
    EXPECT_NEAR(fluid_tracer3.getParticlePosition(0)[0], 0.125, 0.1);
    EXPECT_NEAR(fluid_tracer3.getParticlePosition(0)[1], 0.1, 0.1);
    EXPECT_NEAR(fluid_tracer3.getParticlePosition(1)[0], 0.375, 0.1);
    EXPECT_NEAR(fluid_tracer3.getParticlePosition(1)[1], 0.1, 0.1);

}

TEST(FluidTracer, GridSingleCellUnevenDxDy){
    std::array<int,2> n_cells = {1, 1};
    std::array<double,2> meshWidth1 = {1.0, 0.5};
    std::shared_ptr<Mask> mask = std::make_shared<Mask>(n_cells);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(n_cells, meshWidth1);
    int numParticlesPerCell = 16;
    FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);
}


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

TEST(FluidTracer, updatePositionFluidOnly){
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


// TEST(FluidTracer, updatePositionWithBorderCollision){
//     std::array<int,2> n_cells = {1, 1};
//     std::array<double,2> meshWidth = {1.0, 1.0};
//     std::shared_ptr<Mask> mask = std::make_shared<Mask>(n_cells);
//     std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(n_cells, meshWidth);
//     double vel_x = 1.5;
//     discretization->u(0,0) = vel_x;
//     discretization->u(1,0) = vel_x;
//     discretization->u(0,1) = vel_x;
//     discretization->u(1,1) = vel_x;
//     discretization->u(0,2) = vel_x;
//     discretization->u(1,2) = vel_x;
//     int numParticlesPerCell = 1;
//     FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);
//     EXPECT_EQ(fluid_tracer.getNumberOfParticles(), 1);
//     double old_pos_x = fluid_tracer.getParticlePosition(0)[0];
//     double dt = 1;
//     fluid_tracer.updatePosition(dt);
//     EXPECT_EQ(fluid_tracer.getParticlePosition(0)[0], 0.5);
// }


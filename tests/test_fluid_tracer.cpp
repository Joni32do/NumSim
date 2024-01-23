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
};

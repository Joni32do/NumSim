#include <gtest/gtest.h>
#include "../src/discretization/central_differences.h"

TEST(CentralDifferences, ComputeDu2Dx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    CentralDifferences cd_1(n_cells, meshWidth);
    cd_1.u(1,0) = 1;
    cd_1.u(0,1) = -1;

    EXPECT_EQ(cd_1.computeDu2Dx(1, 1), 0.0);
};

#include <gtest/gtest.h>
#include "../src/discretization/donor_cell.h"

TEST(DonorCell, ComputeDu2Dx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    double alpha = 0.5;
    DonorCell dc_1(n_cells, meshWidth, alpha);
    dc_1.u(0,1) = -1;
    dc_1.u(1,1) = 0;
    dc_1.u(2,1) = 1;

    EXPECT_EQ(dc_1.computeDu2Dx(1, 1), 0.0);
};

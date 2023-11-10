#include <gtest/gtest.h>
#include "../src/discretization/donor_cell.h"

// Pressure terms

// TODO: Add tests for different alphas

TEST(DonorCell, ComputeDpDx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};

    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.p(1,1) = 1;
    dc_1.p(2,1) = -1;

    EXPECT_EQ(dc_1.computeDpDx(1, 1), -2.0);
};



TEST(DonorCell, ComputeDpDy){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.p(1,1) = 1;
    dc_1.p(1,2) = -1;

    EXPECT_EQ(dc_1.computeDpDy(1, 1), -2.0);
};

// Diffusion terms

TEST(DonorCell, ComputeD2uDx2){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(1,2) = 1;
    dc_1.u(2,2) = -1;
    dc_1.u(3,2) = 1;

    EXPECT_EQ(dc_1.computeD2uDx2(2, 2), 4.0);
};


TEST(DonorCell, ComputeD2uDy2){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(2,1) = 1;
    dc_1.u(2,2) = -1;
    dc_1.u(2,3) = 1;

    EXPECT_EQ(dc_1.computeD2uDy2(2, 2), 4.0);
};


TEST(DonorCell, ComputeD2vDx2){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.v(1,2) = 1;
    dc_1.v(2,2) = -1;
    dc_1.v(3,2) = 1;

    EXPECT_EQ(dc_1.computeD2vDx2(2, 2), 4.0);
};


TEST(DonorCell, ComputeD2vDy2){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.v(2,1) = 1;
    dc_1.v(2,2) = -1;
    dc_1.v(2,3) = 1;

    EXPECT_EQ(dc_1.computeD2vDy2(2, 2), 4.0);
};

// Convection terms

TEST(DonorCell, ComputeDu2Dx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(1,2) = -1;
    dc_1.u(2,2) = 0;
    dc_1.u(3,2) = 1;

    EXPECT_EQ(dc_1.computeDu2Dx(2, 2), 0.0);
};


TEST(DonorCell, ComputeDv2Dy){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(2,1) = -1;
    dc_1.u(2,2) = 0;
    dc_1.u(2,3) = 1;

    EXPECT_EQ(dc_1.computeDv2Dy(2, 2), 0.0);
};


TEST(DonorCell, ComputeDuvDy){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(2,2) = 0;
    
    dc_1.v(2,2) = -1;
    dc_1.v(3,2) = -1;
    // -> v(2.5, 2) = -1
    dc_1.u(2,3) = 1;
    // -> u(2,2.5) = 0.5
    double left = -0.5;

    dc_1.v(2,1) = -1;
    dc_1.v(3,1) = -1;
    // -> v(2.5, 1) = -1
    dc_1.u(2,1) = -1;
    // -> u(2,1.5) = -0.5
    double right = 0.5;
    double expected = (left - right) / (meshWidth[1]);

    EXPECT_EQ(dc_1.computeDuvDy(2, 2), expected);
};


TEST(DonorCell, ComputeDuvDx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.v(2,2) = 0;
    
    dc_1.u(2,2) = -1;
    dc_1.u(2,3) = -1;
    // -> u(2, 2.5) = -1
    dc_1.v(3,2) = -1;
    // -> u(2.5,2) = -0.5
    double left = 0.5;

    dc_1.u(1,2) = -1;
    dc_1.u(1,3) = -1;
    // -> u(1, 2.5) = -1
    dc_1.v(1,2) = 1;
    // -> v(1.5,2) = 0.5
    double right = -0.5;

    double expected = (left - right) / (meshWidth[0]);

    EXPECT_EQ(dc_1.computeDuvDx(2, 2), expected);
};
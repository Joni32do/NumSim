#include <gtest/gtest.h>
#include "../src/discretization/central_differences.h"


TEST(CentralDifferences, ComputeDpDx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    CentralDifferences cd_1(n_cells, meshWidth);
    cd_1.p(1,1) = 1;
    cd_1.p(2,1) = -1;

    EXPECT_EQ(cd_1.computeDpDx(1, 1), -2.0);
};



TEST(CentralDifferences, ComputeDpDy){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    CentralDifferences cd_1(n_cells, meshWidth);
    cd_1.p(1,1) = 1;
    cd_1.p(1,2) = -1;

    EXPECT_EQ(cd_1.computeDpDy(1, 1), -2.0);
};





// Convection terms

TEST(CentralDifferences, ComputeDu2Dx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    CentralDifferences cd_1(n_cells, meshWidth);
    cd_1.u(1,2) = -1;
    cd_1.u(2,2) = 0;
    cd_1.u(3,2) = 1;

    EXPECT_EQ(cd_1.computeDu2Dx(2, 2), 0.0);
};


TEST(CentralDifferences, ComputeDv2Dy){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    CentralDifferences cd_1(n_cells, meshWidth);
    cd_1.u(2,1) = -1;
    cd_1.u(2,2) = 0;
    cd_1.u(2,3) = 1;

    EXPECT_EQ(cd_1.computeDv2Dy(2, 2), 0.0);
};


TEST(CentralDifferences, ComputeDuvDy){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    CentralDifferences cd_1(n_cells, meshWidth);
    cd_1.u(2,2) = 0;
    
    cd_1.v(2,2) = -1;
    cd_1.v(3,2) = -1;
    // -> v(2.5, 2) = -1
    cd_1.u(2,3) = 1;
    // -> u(2,2.5) = 0.5
    double left = -0.5;

    cd_1.v(2,1) = -1;
    cd_1.v(3,1) = -1;
    // -> v(2.5, 1) = -1
    cd_1.u(2,1) = -1;
    // -> u(2,1.5) = -0.5
    double right = 0.5;
    double expected = (left - right) / (meshWidth[1]);

    EXPECT_EQ(cd_1.computeDuvDy(2, 2), -1.0);
};


TEST(CentralDifferences, ComputeDuvDx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    CentralDifferences cd_1(n_cells, meshWidth);
    cd_1.v(2,2) = 0;
    
    cd_1.u(1,2) = -1;
    cd_1.u(1,3) = -1;
    // -> u(1, 2.5) = -1
    cd_1.v(1,2) = 1;
    // -> v(1.5,2) = 0.5
    double left = -0.5;

    cd_1.u(2,2) = -1;
    cd_1.u(2,3) = -1;
    // -> u(2, 2.5) = -1
    cd_1.v(3,2) = -1;
    // -> u(2.5,2) = -0.5
    double right = 0.5;
    double expected = (left - right) / (meshWidth[1]);

    EXPECT_EQ(cd_1.computeDuvDy(2, 2), -1.0);
};


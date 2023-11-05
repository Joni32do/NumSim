#include <gtest/gtest.h>
#include "../src/discretization/staggered_grid.h"

TEST(StaggeredGrid, ExampleReadTheDocsGrid){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    StaggeredGrid grids(n_cells, meshWidth);
    grids.u(1,0) = 1;
    grids.u(0,1) = -1;
    grids.v(1,0) = 1;
    grids.v(0,1) = -1;  
    grids.p(1,0) = 1;
    grids.p(0,1) = -1;
    grids.p(1,1) = 4;

    EXPECT_EQ(grids.u().interpolateAt(0.0, 0.0), -0.5);
    EXPECT_EQ(grids.v().interpolateAt(0.0, 0.0), 0.5);
    EXPECT_EQ(grids.p().interpolateAt(0.0, 0.0), 1.0);

    EXPECT_EQ(grids.uIEnd(), 4);
    EXPECT_EQ(grids.uJEnd(), 4);
    EXPECT_EQ(grids.vIEnd(), 5);
    EXPECT_EQ(grids.vJEnd(), 3);
    EXPECT_EQ(grids.pIEnd(), 5);
    EXPECT_EQ(grids.pJEnd(), 4);
};

TEST(StaggeredGrid, MinimalGrid){
    // A grid of size 1x1 is not possible
    std::array<int,2> n_cells = {2, 2};
    std::array<double,2> meshWidth = {1.0, 1.0};
    StaggeredGrid grids(n_cells, meshWidth);
    grids.u(0,0) = -0.5;
    grids.v(0,0) = 0.5;
    grids.p(0,0) = 1.0;


    EXPECT_EQ(grids.u().interpolateAt(0.0, -0.5), -0.5);
    EXPECT_EQ(grids.v().interpolateAt(-0.5, 0.0), 0.5);
    EXPECT_EQ(grids.p().interpolateAt(-0.5, -0.5), 1.0);
};


TEST(StaggeredGrid, LargeGrid){
    int N = 1000;
    std::array<int,2> n_cells = {N, N};
    std::array<double,2> meshWidth = {0.001, 0.001};
    StaggeredGrid grids(n_cells, meshWidth);
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            grids.u(i,j) = i;
            grids.v(i,j) = j;
            grids.p(i,j) = i + j;
        }
    }

    EXPECT_EQ(grids.u().interpolateAt(0.0, -0.0005), 0.0);
    EXPECT_EQ(grids.v().interpolateAt(-0.0005, 0.0), 0.0);
    EXPECT_EQ(grids.p().interpolateAt(-0.0005, -0.0005), 0.0);
};
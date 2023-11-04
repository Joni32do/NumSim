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

TEST(StaggeredGrid, SimpleGrid){
    //TODO:
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
};
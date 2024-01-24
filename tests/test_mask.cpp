#include <gtest/gtest.h>
#include "../src/boundary/mask.h"

TEST(Mask, ConstructorInitAllInnerCellsAsFluid){
    std::array<int,2> size = {3,3};
    Mask mask(size);
    for (int i = 1; i < 4; i++){
        for (int j = 1; j < 4; j++){ 
            EXPECT_EQ(mask(i,j), Mask::FLUID);
        }
    }
};

TEST(Mask, SetOriginToAir){
    std::array<int,2> size = {3,3};
    Mask mask(size);
    mask(0,0) = Mask::AIR;
    EXPECT_EQ(mask(0,0), Mask::AIR);
};

TEST(Mask, isFluid){
    std::array<int,2> size = {3,3};
    Mask mask(size);
    mask(0,0) = Mask::AIR;
    EXPECT_FALSE(mask.isFluid(0,0));
    EXPECT_TRUE(mask.isFluid(1,1));
};

TEST(Mask, isObstacle){
    std::array<int,2> size = {5,5};
    Mask mask(size);
    mask(1,1) = Mask::AIR;
    mask(3,3) = Mask::OBSTACLE;
    EXPECT_TRUE(mask.isObstacle(0,0));
    EXPECT_FALSE(mask.isObstacle(1,1));
    EXPECT_FALSE(mask.isObstacle(2,2));
    EXPECT_TRUE(mask.isObstacle(3,3));
};

TEST(Mask, updateMaskBoundaries){
    // Create simple map
    
    // O O O O O O O
    // O A A A F F O
    // O A A A F F O
    // O A A A F F O
    // O F F F F F O
    // O F F F F F O
    // O O O O O O O

    // O - Obstacle
    // F - Fluid
    // A - Air
    std::array<int,2> size = {5,5};
    Mask mask(size);
    mask(1,5) = Mask::AIR;
    mask(2,5) = Mask::AIR;
    mask(3,5) = Mask::AIR;
    mask(1,4) = Mask::AIR;
    mask(2,4) = Mask::AIR;
    mask(3,4) = Mask::AIR;
    mask(1,3) = Mask::AIR;
    mask(2,3) = Mask::AIR;
    mask(3,3) = Mask::AIR;
    

    mask.updateMaskBoundaries();
    EXPECT_EQ(mask(3,2), Mask::FLUID_BORDER_TOP);
    EXPECT_EQ(mask(4,2), Mask::FLUID);
    EXPECT_EQ(mask(4,3), Mask::FLUID_BORDER_LEFT);

    mask.printMask();
};

TEST(Mask, makeSmallRectangularObstacleCenter){
    // Create simple map
    
    // 130 131 131 131 131 131 131 131 131 131 131 132 
    // 130 015 015 015 015 015 015 015 015 015 015 132 
    // 130 015 015 015 015 015 015 015 015 015 015 132 
    // 130 015 015 015 015 015 015 015 015 015 015 132 
    // 130 015 015 015 015 015 015 015 015 015 015 132 
    // 130 015 015 015 015 121 122 015 015 015 015 132 
    // 130 015 015 015 015 120 123 015 015 015 015 132 
    // 130 015 015 015 015 015 015 015 015 015 015 132 
    // 130 015 015 015 015 015 015 015 015 015 015 132 
    // 130 015 015 015 015 015 015 015 015 015 015 132 
    // 130 015 015 015 015 015 015 015 015 015 015 132 
    // 130 133 133 133 133 133 133 133 133 133 133 132

    // O - Obstacle
    // F - Fluid
    // A - Air
    std::array<int,2> size = {10,10};
    Mask mask(size);
    // std::array<double, 2> meshWidth = {1.0, 1.0};
    // std::array<double, 2> physicalSize = {1.0, 1.0};

    std::array<double, 2> obstaclePosition = {0.45, 0.45};
    std::array<double, 2> obstacleSize = {0.1, 0.1};

    mask.makeRectangularObstacle(obstaclePosition, obstacleSize);
    
    EXPECT_EQ(mask(5,5), Mask::OBSTACLE_CORNER_BOTTOM_LEFT);
    EXPECT_EQ(mask(5,6), Mask::OBSTACLE_CORNER_TOP_LEFT);
    EXPECT_EQ(mask(6,5), Mask::OBSTACLE_CORNER_BOTTOM_RIGHT);
    EXPECT_EQ(mask(6,6), Mask::OBSTACLE_CORNER_TOP_RIGHT);


    mask.printMask();
};


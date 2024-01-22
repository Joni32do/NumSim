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

TEST(Mask, SetAndGet){
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
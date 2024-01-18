#include <gtest/gtest.h>
#include "../src/storage/mask.h"

TEST(Mask, ConstructorInitAllAsFluid){
    std::array<int,2> size = {3,3};
    Mask mask(size);
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){ 
            EXPECT_EQ(mask(i,j), FLUID);
        }
    }
};

TEST(Mask, SetAndGet){
    std::array<int,2> size = {3,3};
    Mask mask(size);
    mask(0,0) = AIR;
    EXPECT_EQ(mask(0,0), AIR);
};

TEST(Mask, isFluid){
    std::array<int,2> size = {3,3};
    Mask mask(size);
    mask(0,0) = AIR;
    EXPECT_FALSE(mask.isFluid(0,0));
    EXPECT_TRUE(mask.isFluid(1,1));
};

TEST(Mask, isObstacle){
    std::array<int,2> size = {5,5};
    Mask mask(size);
    mask(1,1) = AIR;
    mask(3,3) = OBSTACLE;
    EXPECT_TRUE(mask.isObstacle(0,0));
    EXPECT_FALSE(mask.isObstacle(1,1));
    EXPECT_FALSE(mask.isObstacle(2,2));
    EXPECT_TRUE(mask.isObstacle(3,3));
};

TEST(Mask, updateMaskBoundaries){
    // A A F F F
    // A A A F F
    // A F A F F
    // F F F A F
    // F F F F F
    std::array<int,2> size = {5,5};
    Mask mask(size);
    mask(0,4) = AIR;
    mask(1,4) = AIR;
    mask(0,3) = AIR;
    mask(1,3) = AIR;
    mask(2,3) = AIR;
    mask(0,2) = AIR;
    mask(2,2) = AIR;
    mask(3,1) = AIR;

    mask.updateMaskBoundaries();
    EXPECT_EQ(mask(0,1), FLUID_BORDER_TOP);
    EXPECT_EQ(mask(1,1), FLUID);
    EXPECT_EQ(mask(1,2), FLUID_SINGLE_BOTTOM);
    EXPECT_EQ(mask(2,1), FLUID_CORNER_TOP_RIGHT);
    EXPECT_EQ(mask(2,2), FLUID_COLUMN_HORIZONTAL);

    mask.printMask();
};
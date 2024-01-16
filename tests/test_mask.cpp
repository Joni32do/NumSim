#include <gtest/gtest.h>
#include "../src/storage/mask.h"

TEST(Mask, Constructor){
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
    std::array<int,2> size = {3,3};
    Mask mask(size);
    mask(0,0) = OBSTACLE;
    mask(1,1) = AIR;
    EXPECT_TRUE(mask.isObstacle(0,0));
    EXPECT_FALSE(mask.isObstacle(1,1));
    EXPECT_FALSE(mask.isObstacle(2,2));
};
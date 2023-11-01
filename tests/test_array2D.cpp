#include <gtest/gtest.h>
#include "../src/storage/array2D.h"

TEST(Array2D, Constructor){
    std::array<int,2> size = {2,3};	
    Array2D a(size);
    EXPECT_EQ(a.size(), size);
    EXPECT_EQ(a.size()[0], 2);
    EXPECT_EQ(a.size()[1], 3);
};

TEST(Array2D, SetValue){
    std::array<int,2> size = {2,3};	
    Array2D a(size);
    a(1,1) = 1.0;
    EXPECT_EQ(a(1,1), 1.0);
};

TEST(Array2D, InitializeArrayWithZeros){
    std::array<int,2> size = {2,3};	
    Array2D a(size);
    EXPECT_EQ(a(0,0), 0.0);
};

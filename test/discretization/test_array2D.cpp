#include <gtest/gtest.h>
#include "../../src/discretization/array2D/array2D.h"

TEST(Array2D, Constructor){
    std::array<int,2> size = {2,3};	
    Array2D a(size);
    EXPECT_EQ(a.size(), {2,3});
    EXPECT_EQ(a.size()[0], 2);
    EXPECT_EQ(a.size()[0], 3);
};

TEST(Array2D, SetValue(){
    std::array<int,2> size = {2,3};	
    Array2D a(size);
    a(1,1) = 1.0;
    EXPECT_EQ(a.data_[2]), 1.0);
};

TEST(Array2D, GetValue){
    std::array<int,2> size = {2,3};	
    Array2D a(size);
    a.data_[2] = 1.0;
    EXPECT_EQ(a(1,1), 1.0);
};

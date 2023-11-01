#include <gtest/gtest.h>
#include "../src/storage/field_variable.h"

TEST(FieldVariable, Constructor){
    std::array<int,2> size = {3,3};	
    std::array<double,2> origin = {0.0, 0.0};
    std::array<double,2> meshWidth = {0.5, 0.5};
    FieldVariable field(size, origin, meshWidth);
};

TEST(FieldVariable, InterpolateAtMiddle){
    std::array<int,2> size = {2,2};	
    std::array<double,2> origin = {0.0, 0.0};
    std::array<double,2> meshWidth = {1.0, 1.0};
    FieldVariable field(size, origin, meshWidth);
    field(0,0) = -5.0;
    field(1,1) = 5.0;
    EXPECT_EQ(field.interpolateAt(0.5, 0.5), 0.0);
    EXPECT_EQ(field.interpolateAt(0.0, 0.0), -5.0);
    EXPECT_EQ(field.interpolateAt(1.0, 1.0), 5.0);
};
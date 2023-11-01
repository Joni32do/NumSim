#include "array2D.h"
#include "field_variable.h"
#include <cmath>


FieldVariable::FieldVariable(std::array<int,2> size,
                    std::array<double,2> origin,
                    std::array<double,2> meshWidth):
    Array2D::Array2D(size),
    origin_(origin),
    meshWidth_(meshWidth)
{
}

double FieldVariable::interpolateAt(double x, double y) const{
    // reshape x and y to local coordinates
    double x_0 = x - origin_[0];
    double y_0 = y - origin_[1];

    // get the floating point indizi
    double i_float = x_0 / meshWidth_[0];
    double j_float = y_0 / meshWidth_[1];

    // floored indizi
    int i = std::floor(i_float);
    int j = std::floor(j_float);

    // case 1: x and y are on a node

    if (i_float == double(i) && j_float == double(j)){
        return (*this)(i, j);
    } else {
        // case 2: bilinear interpolation between four nodes
        double x_weight = i_float - i;
        double y_weight = j_float - j;
        // value lower left node
        double down_left = (1-x_weight)*(*this)(i,j);
        double down_right = (x_weight)*(*this)(i+1,j);
        double up_left = (1-x_weight)*(*this)(i,j+1);
        double up_right = (x_weight)*(*this)(i+1,j+1);
        return (1-y_weight)*(down_left + down_right) + y_weight*(up_left + up_right);
    }
}

   

    
    


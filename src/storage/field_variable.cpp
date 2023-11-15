#include "array2D.h"
#include "field_variable.h"

#include <iostream>


FieldVariable::FieldVariable(std::array<int,2> size,
                    std::array<double,2> origin,
                    std::array<double,2> meshWidth):
    Array2D::Array2D(size),
    origin_(origin),
    meshWidth_(meshWidth)
{
}


double FieldVariable::interpolateAt(double x, double y) const{

    //get position of point, relativ to the origin
    double s = x-origin_[0];
    double t = y-origin_[1];

    // get the lower index of the cell the point is in 
    int lowerx = floor(s/meshWidth_[0]);
    int lowery = floor(t/meshWidth_[1]);
    
    //assert if Point (x,y) is outside of grid
    assert(0 <= lowerx && lowerx+1 < size_[0]);
    assert(0 <= lowery && lowery+1 < size_[1]);

    // compute the coordinates of the 4 corners of the cell
    double x1 = lowerx * meshWidth_[0];
    double x2 = x1 + meshWidth_[0];
    double y1 = lowery * meshWidth_[1];
    double y2 = y1 + meshWidth_[1];

    //compute the weights the 4 corners get:
    double w11 = (x2-s)*(y2-t)/((x2-x1)*(y2-y1));
    double w12 = (x2-s)*(t-y1)/((x2-x1)*(y2-y1));
    double w21 = (s-x1)*(y2-t)/((x2-x1)*(y2-y1));
    double w22 = (s-x1)*(t-y1)/((x2-x1)*(y2-y1));
    
    double value = w11*(*this)(lowerx,lowery) + w12*(*this)(lowerx,lowery+1) + w21*(*this)(lowerx+1,lowery) + w22*(*this)(lowerx+1,lowery+1); 
    //return weighted value
    return value;
}

// double FieldVariable::interpolateAt(double x, double y) const{
//     // reshape x and y to local coordinates
//     double x_0 = x - origin_[0];
//     double y_0 = y - origin_[1];

//     // get the floating point indizi
//     double i_float = x_0 / meshWidth_[0];
//     double j_float = y_0 / meshWidth_[1];

//     // floored indizi
//     int i = (int) i_float;
//     int j = (int) j_float;

//     double i_weight = i_float - i;
//     double j_weight = j_float - j;

//     // case 1: node value
//     if (i_weight == 0.0 && j_weight == 0.0){
//         return (*this)(i, j);
//     // case 2: linear interpolation between y-values
//     } else if (i_weight == 0.0 && j_weight != 0.0){
//         return (1-j_weight)*(*this)(i,j) + j_weight*(*this)(i,j+1);
//     // case 3: linear interpolation between x-values
//     } else if (i_weight != 0.0 && j_weight == 0.0){
//         return (1-i_weight)*(*this)(i,j) + i_weight*(*this)(i+1,j);
//     // case 4: bilinear interpolation between four nodes
//     } else {
//         double down_left = (1-i_weight)*(*this)(i,j);
//         double down_right = (i_weight)*(*this)(i+1,j);
//         double up_left = (1-i_weight)*(*this)(i,j+1);
//         double up_right = (i_weight)*(*this)(i+1,j+1);
//         return (1-j_weight)*(down_left + down_right) + j_weight*(up_left + up_right);
//     }
// }


double FieldVariable::findAbsMax() const{
    double max = 0;
    for (int i = 0; i < size_[0]; i++){
        for (int j = 0; j < size_[1]; j++){
            if (std::abs((*this)(i,j)) > max){
                max = std::abs((*this)(i,j));
            }
        }
    }
    return max;
}

   

    
    


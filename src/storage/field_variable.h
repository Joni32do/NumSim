#pragma once

#include <array>
#include <vector>
#include <cmath>
#include "array2D.h"

/**
 * @class FieldVariable
 * @brief 
 * 
 * A field variable is the discretization of a scalar function f(x) with x in the computational domain. 
 * More specifically, a scalar value is stored at discrete nodes/points. 
 * The nodes are arranged in an equidistant mesh with specified mesh width.
*/
class FieldVariable: public Array2D
{
    public:
        FieldVariable(std::array<int,2> size, std::array<double,2> origin, std::array<double,2> meshWidth);
        /**
         * @brief get the value at the Cartesian coordinate (x,y). The value is linearly interpolated between stored points. 
        */ 
        double interpolateAt(double x, double y) const;

        double findAbsMax() const;
    
    private:
        const std::array<double,2> origin_;
        const std::array<double,2> meshWidth_;

};
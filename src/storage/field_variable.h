#pragma once

#include <array>
#include <vector>
#include <cmath>
#include "array2D.h"

/**
 * @class FieldVariable
 * @brief Discretization of a scalar function
 * 
 * A field variable is the discretization of a scalar function f(x) with x in the computational domain. 
 * More specifically, a scalar value is stored at discrete nodes/points. 
 * The nodes are arranged in an equidistant mesh with specified mesh width.
*/
class FieldVariable: public Array2D
{
    public:
        /**
         * @brief Constructor. 
         * 
         * @param size number of cells in x and y direction 
         * @param origin orgin of Fieldvariable relative to domain origin
         * @param meshwidth mesh width in x and y direction  
        */ 
        FieldVariable(std::array<int,2> size, std::array<double,2> origin, std::array<double,2> meshWidth);
        
        /**
         * @brief Interpolates value of field variable in domain using x and y coordinates
         * 
         * Interpolates by using bilinear interpolation. 
         *  
         * @param x x postion of point to interpolate
         * @param y y postion of point to interpolate
        */ 
        double interpolateAt(double x, double y) const;

        /**
         * @brief Find point in Array2D of Fieldvariable with maximum value
         * 
         * Finds maximum by iterating through all values in FielVariable 
        */ 
        double findAbsMax() const;
    
    private:
        const std::array<double,2> origin_;   //!< origin of field variable relative to domain
        const std::array<double,2> meshWidth_;  //!< mesh width of FieldVariable in x and y direction 

};
#pragma once

#include <vector>
#include <array>
#include <cassert>


/** This class represents a 2D array of double values.
 *  Internally they are stored consecutively in memory.
 *  The entries can be accessed by two indices i,j.
 */
class Array2D
{
    public:
        Array2D(std::array<int, 2> size);

        //! set value
        double &operator()(int i, int j);
        
        //! get value
        double operator()(int i, int j) const;

        //! get size
        std::array<int,2> size() const;

    protected:
        const std::array<int,2> size_;    //< width, height of the domain
        std::vector<double> data_;  //< storage array values, in row-major order
};
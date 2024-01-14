#pragma once

#include "array2D.h"

enum CellType{
    FLUID = 0,
    OBSTACLE = 10,
    OBSTACLE_BORDER_LEFT = 11,
    OBSTACLE_BORDER_TOP = 12,
    OBSTACLE_BORDER_RIGHT = 13,
    OBSTACLE_BORDER_BOTTOM = 14,
    OBSTACLE_CORNER_BOTTOM_LEFT = 15,
    OBSTACLE_CORNER_TOP_LEFT = 16,
    OBSTACLE_CORNER_TOP_RIGHT = 17,
    OBSTACLE_CORNER_BOTTOM_RIGHT = 18
};

class Mask{
    public:
    /**
     * @brief constructor.
     *
     * @param size size of array in x and y direction
     */
    Mask(std::array<int, 2> size);

    /**
     * @brief set array value.
     *        Overloads the () operator
     *
     * @param i index in x direction
     * @param j index in y direction
     */
    double &operator()(int i, int j);

    /**
     * @brief get array value.
     *        Overloads the () operator
     *
     * @param i index in x direction
     * @param j index in y direction
     */
    double operator()(int i, int j) const;

    /**
     * @brief get size of array in x and y direction
     */
    std::array<int, 2> size() const;

protected:
    const std::array<int, 2> size_; //!< size of array in x and y direction
    std::vector<int> data_;      //!< storage array values, in row-major order
};
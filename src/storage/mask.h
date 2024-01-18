#pragma once

#include "array2D.h"

#include <iostream>
#include <fstream>

//TODO: it is weird but if we would use `char` instead of int, we would use less memory

enum CellType{
    DROPLET = 0,
    FLUID = 15,

    FLUID_SINGLE_LEFT = 1,
    FLUID_SINGLE_TOP = 2,
    FLUID_SINGLE_RIGHT = 4,
    FLUID_SINGLE_BOTTOM = 8,

    FLUID_CORNER_BOTTOM_LEFT = 6,
    FLUID_CORNER_TOP_LEFT = 12,
    FLUID_CORNER_TOP_RIGHT = 9,
    FLUID_CORNER_BOTTOM_RIGHT = 3,

    FLUID_COLUMN_HORIZONTAL = 5,
    FLUID_COLUMN_VERTICAL = 10,

    FLUID_BORDER_LEFT = 14,
    FLUID_BORDER_TOP = 13,
    FLUID_BORDER_RIGHT = 11,
    FLUID_BORDER_BOTTOM = 7,

    FLUID_TYPE = 50,
    AIR = 64,

    OBSTACLE = 100,

    OBSTACLE_BORDER_LEFT = 110,
    OBSTACLE_BORDER_TOP = 111,
    OBSTACLE_BORDER_RIGHT = 112,
    OBSTACLE_BORDER_BOTTOM = 113,

    OBSTACLE_CORNER_BOTTOM_LEFT = 120,
    OBSTACLE_CORNER_TOP_LEFT = 121,
    OBSTACLE_CORNER_TOP_RIGHT = 122,
    OBSTACLE_CORNER_BOTTOM_RIGHT = 123,
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
    int &operator()(int i, int j);

    /**
     * @brief get array value.
     *        Overloads the () operator
     *
     * @param i index in x direction
     * @param j index in y direction
     */
    int operator()(int i, int j) const;

    /**
     * @brief get size of array in x and y direction
     */
    std::array<int, 2> size() const;

    /**
     * @brief update boundary of mask after the velocity step
     * 
     * TODO: this can be implemented more efficiently by only updating the cells which are next
     * to a fluid cell, because only those can change their type
     * Could use `isFluidBorder`
     */
    void updateMaskBoundaries();


    /**
     * @brief Returns a boolean indicating whether the cell at (i,j) is fluid or not
     * 
     * @param i index in x direction
     * @param j index in y direction
     * 
     * If i and j are out of bond returns false
     */
    bool isFluid(int i, int j) const;

    /**
     * @brief Returns a boolean indicating whether the cell at (i,j) is an obstacle or not
     * 
     * @param i index in x direction
     * @param j index in y direction
     */
    bool isObstacle(int i, int j) const;

    /**
     * @brief Returns a boolean indicating whether the cell at (i,j) is a border cell or not
     * 
     * @param i index in x direction
     * @param j index in y direction
     */
    bool isFluidBorder(int i, int j) const;

    /**
     * @brief Returns a boolean indicating whether the cell at (i,j) is a air cell or not
     * 
     * @param i index in x direction
     * @param j index in y direction
     * 
     * If i and j are out of bond returns false
     */
    bool isAir(int i, int j) const;

    /**
     * @brief Returns a boolean indicating whether the cell at (i,j) is not a air cell or not
     * 
     * @param i index in x direction
     * @param j index in y direction
     * 
     * If i and j are out of bond returns true
     */
    bool isNotAir(int i, int j) const;





    // void placeRectangle()


    void printMask() const;

    // void createBitmap() const;



protected:
    const std::array<int, 2> size_; //!< size of array in x and y direction
    std::vector<int> data_;      //!< storage array values, in row-major order
    std::vector<int> boundary_list;  //!< stores the indices of the boundary cells
};
#pragma once

#include <vector>
#include <array>
#include <cassert>
#include <cmath>

#include <iostream>
#include <iomanip>
#include <png.h>
#include <fstream>
#include "../settings_parser/settings.h"

class Mask
{
public:
    /**
     * @brief constructor.
     *
     * @param size size of array in x and y direction
     */
    Mask(Settings settings);

    // TODO: it is weird but if we would use `char` instead of int, we would use less memory

    enum CellType
    {
        FLUID_DROPLET = 0,

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

        FLUID = 15,
        FLUID_TYPE = 50,
        AIR = 64,

        OBSTACLE = 99,
        OBSTACLE_INSIDE = 100,

        OBSTACLE_BORDER_LEFT = 101,
        OBSTACLE_BORDER_TOP = 102,
        OBSTACLE_CORNER_TOP_LEFT = 103,
        OBSTACLE_BORDER_RIGHT = 104,
        OBSTACLE_CORNER_TOP_RIGHT = 106,
        OBSTACLE_BORDER_BOTTOM = 108,
        OBSTACLE_CORNER_BOTTOM_LEFT = 109,
        OBSTACLE_CORNER_BOTTOM_RIGHT = 112,

        DOMAIN_BOUNDARY = 200,

        DOMAIN_LEFT_NOSLIP = 201,
        DOMAIN_TOP_NOSLIP = 202,
        DOMAIN_RIGHT_NOSLIP = 203,
        DOMAIN_BOTTOM_NOSLIP = 204,

        DOMAIN_LEFT_PRESSURE = 205,
        DOMAIN_TOP_PRESSURE = 206,
        DOMAIN_RIGHT_PRESSURE = 207,
        DOMAIN_BOTTOM_PRESSURE = 208,
    };

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
     * @brief sets all not obstacle cells to air
     *
     */
    void resetMask();

    // ********************************************************
    // R E Q U E S T S 

    /**
     * @brief Cell at (i,j) is fluid or not
     *
     * @param i index in x direction
     * @param j index in y direction
     *
     * If i and j are out of bond returns false
     */
    bool isFluid(int i, int j) const;

    /**
     * @brief True if Cell at (i,j) is either obstacle or boundary
     *
     * @param i index in x direction
     * @param j index in y direction
     */
    bool isObstacle(int i, int j) const;


    /**
     * @brief Cell at (i,j) is a air cell or not
     *
     * @param i index in x direction
     * @param j index in y direction
     *
     * If i and j are out of bond returns false
     */
    bool isAir(int i, int j) const;
    bool isInnerFluid(int i, int j) const;


    bool isDomainBoundary(int i, int j) const;
    bool isObstacleBoundary(int i, int j) const;
    bool isFluidBoundary(int i, int j) const;


    /**
     * @brief number of fluid cells
     *
     * not efficient but is only used once in the creation of the fluid tracer
     *
     */
    int getNumberOfFluidCells() const;

    // For initialization
    void makeRectangularObstacle();
    void createMaskFromPNGBitMap();




    // Sets Enums for Boundary Types from type FLUID, AIR, OBSTACLE, DOMAIN
    //  - uses `settings` and neighbours
    void setDomainBC();
    void setObstacleBC();
    void setFluidBC();

    void printMask() const;


protected:
    std::array<int, 2> size_; //!< size of array in x and y direction
    std::vector<int> data_;         //!< storage array values, in row-major order
    std::vector<int> boundary_list; //!< stores the indices of the boundary cells
    Settings settings_;
    int forbiddenObstacleFluidCombinations[7] = {105,107,110,111,113,114,115};
};
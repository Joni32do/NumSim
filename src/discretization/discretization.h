#pragma once

#include <array>
#include <vector>
#include "staggered_grid.h"

/**
 * @class Discretization
 * @brief Computes derivatives included in governing equations
 * 
 * Interface for the discretization. Computes the
 * needed derivatives to solve the Poisson and 
 * continuity equations. 
*/
class Discretization : public StaggeredGrid
{
public:
    /**
     * @brief Constructor
     * 
     * @param nCells array containing number of cells in x and y directions
     * @param meshWidth array containing the length of a single cell in x and y directions
    */ 
    Discretization(std::array<int, 2> nCells, std::array<double, 2> meshWidth);

    /**
     * @brief Calculate first derivative of u^2 with respect to x
    */
    virtual double computeDu2Dx(int i, int j) const = 0;
    /**
     * @brief Calculate first derivative of v^2 with respect to y
    */
    virtual double computeDv2Dy(int i, int j) const = 0;

    /**
     * @brief Calculate first derivative of u*v with respect to x
    */
    virtual double computeDuvDx(int i, int j) const = 0;
    /**
     * @brief Calculate first derivative of u*v with respect to y
    */
    virtual double computeDuvDy(int i, int j) const = 0;

    /**
     * @brief Calculate second derivative of u with respect to x
    */
    virtual double computeD2uDx2(int i, int j) const;
    /**
     * @brief Calculate second derivative of u with respect to y
    */
    virtual double computeD2uDy2(int i, int j) const;
    /**
     * @brief Calculate second derivative of v with respect to x
    */
    virtual double computeD2vDx2(int i, int j) const;
    /**
     * @brief Calculate second derivative of v with respect to y
    */
    virtual double computeD2vDy2(int i, int j) const;

    /**
     * @brief Calculate first derivative of p with respect to x
    */
    virtual double computeDpDx(int i, int j) const;
    /**
     * @brief Calculate first derivative of p with respect to y
    */
    virtual double computeDpDy(int i, int j) const;
};
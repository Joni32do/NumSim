#pragma once

#include <array>
#include <vector>
#include "discretization.h"

/**
 * @class CentralDifferences
 * @brief Discretizes the problem using the central differences scheme  
 * 
 * Implements the first derivatives of u^2, v^2 and u*v according
 * to the central differences scheme
*/
class CentralDifferences : public Discretization
{
public:
    /**
     * @brief Constructor
     * 
     * @param nCells array containing number of cells in x and y directions
     * @param meshWidth array containing the length of a single cell in x and y directions
    */ 
    CentralDifferences(std::array<int, 2> nCells, std::array<double, 2> meshWidth);

    /**
     * @brief Calculate first derivative of u^2 with respect to x with the
             central differences Scheme
    */
    double computeDu2Dx(int i, int j) const override;
    /**
     * @brief Calculate first derivative of v^2 with respect to y with the
              central differences Scheme
    */
    double computeDv2Dy(int i, int j) const override;
    /**
     * @brief Calculate first derivative of u*v with respect to x with the
              central differences Scheme
    */
    double computeDuvDx(int i, int j) const override;
    /**
     * @brief Calculate first derivative of u*v with respect to y with the
              central differences Scheme
    */
    double computeDuvDy(int i, int j) const override;
};

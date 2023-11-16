#pragma once

#include <array>
#include <vector>
#include <cmath>
#include "discretization.h"

/**
 * @class DonorCell
 * @brief Discretizes the problem using the donor cell scheme  
 * 
 * Implements the first derivatives of u^2, v^2 and u*v according
 * to the donor cell scheme
*/
class DonorCell : public Discretization
{
public:
    /**
     * @brief Constructor
     * 
     * @param nCells array containing number of cells in x and y directions
     * @param meshWidth array containing the length of a single cell in x and y directions
     * @param alpha weight factor between central differences and donor cell schemes
    */ 
    DonorCell(std::array<int, 2> nCells, std::array<double, 2> meshWidth, double alpha);

    /**
     * @brief Calculate first derivative of u^2 with respect to x with the
              donor cell Scheme
    */
    double computeDu2Dx(int i, int j) const override;
    /**
     * @brief Calculate first derivative of v^2 with respect to y with the
              donor cell Scheme
    */
    double computeDv2Dy(int i, int j) const override;
    /**
     * @brief Calculate first derivative of u*v with respect to x with the
              donor cell Scheme
    */
    double computeDuvDx(int i, int j) const override;
    /**
     * @brief Calculate first derivative of u*v with respect to y with the
              donor cell Scheme
    */
    double computeDuvDy(int i, int j) const override;

private:
    const double alpha_; //!< weight factor between central differences and donor cell schemes
};
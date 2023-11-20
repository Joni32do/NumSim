#pragma once

#include "pressure_solver.h"
#include <iostream>

/**
 * @class GaussSeidel
 * @brief Standard Gauss-Seidel solver
 *
 */
class GaussSeidel : public PressureSolver
{

public:
    /**
     * @brief Constructor.
     *
     * @param data instance of Discretization holding the needed field variables for rhs and p
     * @param epsilon tolerance for the solver
     * @param maximumNumberOfIterations maximum of iteration
     */
    GaussSeidel(const std::shared_ptr<Discretization> &data,
                double epsilon,
                int maximumNumberOfIterations);
    /**
     * @brief override function that starts solver.
     *
     */
    void solve() override;
};
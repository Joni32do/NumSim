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
* @param seettings contains settings for solver
     */
    GaussSeidel(const std::shared_ptr<Discretization> &data,
                Settings settings);
             
    /**
     * @brief override function that starts solver.
     *
     */
    void solve() override;
};
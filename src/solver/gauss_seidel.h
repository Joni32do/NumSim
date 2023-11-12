#pragma once

#include "pressure_solver.h"

/**
 * @brief Standard Gauss-Seidel solver
 * 
 */
class GaussSeidel : public PressureSolver {

public:

    GaussSeidel(const std::shared_ptr<Discretization>& data, 
                        double epsilon, 
                        int maximumNumberOfIterations,
                        std::array<double,2> meshWidth);

    void solve() override;
};
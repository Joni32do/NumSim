#pragma once

#include "pressure_solver.h"

/**
 * @brief Successive over-relaxation solver
 * 
 */
class SOR : public PressureSolver {

public:

    SOR(const std::shared_ptr<Discretization>& data, 
                        double epsilon, 
                        int maximumNumberOfIterations, 
                        double omega,
                        std::array<double, 2> meshWidth);

    void solve() override;

private:
    double omega_;
};
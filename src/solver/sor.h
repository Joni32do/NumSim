#pragma once

#include "pressure_solver.h";

/**
 * @brief Successive over-relaxation solver
 * 
 */
class SOR : public PressureSolver {

public:

    SOR(const std::shared_ptr<Discretization>& data, 
                        double epsilon, 
                        int maximumNumberOfIterations, 
                        double omega);

    void solve() override;

private:
    double omega_;

};
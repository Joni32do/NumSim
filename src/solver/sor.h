#pragma once

#include "pressure_solver.h";

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
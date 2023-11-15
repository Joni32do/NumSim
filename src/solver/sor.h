#pragma once

#include "pressure_solver.h"

/**
 * @class SOR 
 * @brief Successive over-relaxation solver
 * 
*/
class SOR : public PressureSolver {

public:
    /**
     * @brief Constructor. 
     * 
     * @param data instance of Discretization holding the needed field variables for rhs and p
     * @param epsilon tolerance for the solver 
     * @param maximumNumberOfIterations maximum of iteration
     * @param omega relaxation factor
    */ 
    SOR(const std::shared_ptr<Discretization>& data, 
                        double epsilon, 
                        int maximumNumberOfIterations, 
                        double omega);

    /**
     * @brief override function that starts solver. 
     * 
    */ 
    void solve() override;

private:
    double omega_;  //!< relaxation factor for SOR 
};
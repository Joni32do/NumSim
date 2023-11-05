#pragma once

#include <../discretization/discretization.h>
#include <memory>

/**
 * @brief Solves the Poisson problem for the pressure
 * 
 * Interface for the pressure solver. It computes the 
 * pressure field variable such that the continuity equation 
 * is fulfilled. 
 */
class PressureSolver 
{
    public:
        PressureSolver(std::shared_ptr<Discretization> discretization, double epsilon, int maximumNumberOfIterations);

        /**
         * @brief solve the Poisson problem for the pressure
         * 
         */
        virtual void solve() = 0;

    protected:
        /**
         * @brief Set the Boundary Values
         * 
         * Account for homogenous Neuman BC
         * 
         * Has to be called every iteration 
         */
        void setBoundaryValues();

        //! object holding the needed field variables for rhs and p
        std::shared_ptr<Discretization> discretization_;
        //! tolerance for the solver
        double epsilon_;
        //! maximum number of iterations
        int maximumNumberOfIterations_;

};
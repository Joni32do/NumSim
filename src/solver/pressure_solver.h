#pragma once
#include "../storage/field_variable.h"
#include "../discretization/discretization.h"
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
        PressureSolver(std::shared_ptr<Discretization> discretization, 
                       double epsilon, 
                       int maximumNumberOfIterations);

        /**
         * @brief solve the Poisson problem for the pressure
         * 
         */
        virtual void solve() = 0;

        double dx2, dy2; // squared mesh widths

    protected:
        /**
         * @brief Set the Boundary Values
         * 
         * Account for homogenous Neuman BC
         * Has to be called every iteration 
         */
        void setBoundaryValues();

        // calculate residuum of current time step
        double calculateResiduum();

        // boundaries for p
        int i_beg; // begin of loop for p in x direction
        int i_end; // end   of loop for p in x direction
        int j_beg; // begin of loop for p in y direction
        int j_end; // end   of loop for p in y direction

        //! object holding the needed field variables for rhs and p
        std::shared_ptr<Discretization> discretization_;
        //! tolerance for the solver
        double epsilon_;
        //! maximum number of iterations
        int maximumNumberOfIterations_;

};
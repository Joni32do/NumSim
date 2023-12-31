#pragma once
#include "../storage/field_variable.h"
#include "../discretization/discretization.h"
#include <memory>

/**
 * @class PressureSolver
 * @brief Solves the Poisson problem for the pressure
 *
 * Interface for the pressure solver. It computes the
 * pressure field variable such that the continuity equation
 * is fulfilled.
 */
class PressureSolver
{
public:
    /**
     * @brief Constructor.
     *
     * @param discretization instance of Discretization holding the needed field variables for rhs and p
     * @param epsilon tolerance for the solver
     * @param maximumNumberOfIterations maximum of iteration
     */
    PressureSolver(std::shared_ptr<Discretization> discretization,
                   double epsilon,
                   int maximumNumberOfIterations);

    /**
     * @brief virtual function that starts solver.
     *
     */
    virtual void solve() = 0;

    double dx2, dy2; //!< squared mesh widths

protected:
    /**
     * @brief Set the Boundary Values
     *
     * Account for homogenous Neuman BC
     * Has to be called every iteration
     */
    void setBoundaryValues();

    /**
     * @brief calculate residuum of current time step
     */
    double calculateResiduum();

    int i_beg; //!< begin of loop for rhs in x direction
    int i_end; //!< end   of loop for rhs in x direction
    int j_beg; //!< begin of loop for rhs in y direction
    int j_end; //!< end   of loop for rhs in y direction

    std::shared_ptr<Discretization> discretization_; //!< object holding the needed field variables for rhs and p

    double epsilon_; //!< tolerance for the solver

    int maximumNumberOfIterations_; //!< maximum number of iterations
};
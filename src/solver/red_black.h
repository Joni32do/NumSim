#pragma once

#include "pressure_solver.h"
#include "../parallel/communicator.h"
#include "../parallel/partitioning.h"

#include <vector>

/**
 * @class RedBlack
 *
 */
class RedBlack : public PressureSolver
{

public:
    /**
     * @brief Constructor.
     *
     * @param data instance of Discretization holding the needed field variables for rhs and p
     * @param epsilon tolerance for the solver
     * @param maximumNumberOfIterations maximum of iteration
     * @param omega relaxation factor
     */
    RedBlack(const std::shared_ptr<Discretization> &data,
        double epsilon,
        int maximumNumberOfIterations,
        std::shared_ptr<Communicator> communicator,
        std::shared_ptr<Partitioning> partitioning);

    /**
     * @brief override function that starts solver.
     *
     */
    void solve() override;

    void setBoundaryValues();

    void exchangeGhost();

private:
    std::shared_ptr<Communicator> communicator_;
    std::shared_ptr<Partitioning> partitioning_;
};
#pragma once

#include "pressure_solver.h"
#include "../parallel/communicator.h"
#include "../parallel/partitioning.h"

#include <iostream>
#include <sstream>
#include <iomanip>

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

    /**
     * @brief set boundary values of pressure.
     *
     */
    void setBoundaryValues();

    /**
     * @brief exchange ghost.
     *
     */
    void exchangeGhost(int current_it);

private:
    std::shared_ptr<Communicator> communicator_; //!< comminucator instance
    std::shared_ptr<Partitioning> partitioning_; //!< partitioning instance

    std::vector<double> bufferReceiveBottom; //!< bufferReceiveBottom vector
    std::vector<double> bufferReceiveTop;    //!< bufferReceiveTop vector
    std::vector<double> bufferReceiveLeft;   //!< bufferReceiveLeft vector
    std::vector<double> bufferReceiveRight;  //!< bufferReceiveRight vector

    std::vector<double> bufferBottom; //!< bufferBottom vector
    std::vector<double> bufferTop;    //!< bufferTop vector
    std::vector<double> bufferLeft;   //!< bufferLeft vector
    std::vector<double> bufferRight;  //!< bufferRight vector
};
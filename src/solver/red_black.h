#pragma once

#include "pressure_solver.h"
#include "../parallel/communicator.h"
#include "../parallel/partitioning.h"
#include "../debugging/debug_printer.h"

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
        std::shared_ptr<Partitioning> partitioning,
        std::shared_ptr<Printer> printer);

    /**
     * @brief override function that starts solver.
     *
     */
    void solve() override;

    void setBoundaryValues();

    /**
     * @brief exchanges the whole boundary rows and cols of neighbouring processes
    */
    void exchangeGhost();

    /**
     * @brief only exchanges half the boundary according to which value is required for the next red-black step
    */
    void exchangeGhost(int startShift);

private:
    std::shared_ptr<Communicator> communicator_;
    std::shared_ptr<Partitioning> partitioning_;
    std::shared_ptr<Printer> printer_;

    std::vector<double> bufferReceiveBottom;
    std::vector<double> bufferReceiveTop;
    std::vector<double> bufferReceiveLeft;
    std::vector<double> bufferReceiveRight;

    std::vector<double> bufferBottom;
    std::vector<double> bufferTop;
    std::vector<double> bufferLeft;
    std::vector<double> bufferRight;

};
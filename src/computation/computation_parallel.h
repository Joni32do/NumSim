#pragma once

#include <memory>
#include <cmath>

#include "computation/computation.h"
#include "parallel/communicator.h"
#include "parallel/partitioning.h"

#include "debugging/debug_printer.h"
#include "output_writer/output_writer_text_parallel.h"
#include "output_writer/output_writer_paraview_parallel.h"
#include "solver/red_black.h"

/**
 * @class Computation
 * @brief Computation of Simulation
 *
 * Combines discretization, pressure solver and outputwriter (parallel)
 */
class ComputationParallel : Computation
{

public:
    /**
     * @brief Initialize the simulation with settings from .txt file
     *
     * @param argc counts of CLI commands
     * @param argv values of CLI commands, expect settings.txt filename
     */
    void initializeParallel(int argc, char *argv[]);

    /**
     * @brief run the whole simulation until t_end
     *
     */
    void runSimulationParallel();

    /**
     * @brief Set boundary values of u and v to correct values
     */
    void applyBoundaryValuesParallel();

    /**
     * @brief Compute the time step width dt from maximum velocities
     *
     * Uses CFL condition
     */
    void computeTimeStepWidthParallel(double currentTime);

    /**
     * @brief Compute the preliminary velocities F and G
     */
    void computePreliminaryVelocitiesParallel();

    /**
     * @brief Compute the right hand side of the pressure equation
     */
    void computeRightHandSideParallel();

    /**
     * @brief Compute the new velocities, u, v, from the preliminary
     *        velocities F, G and the pressure.
     */
    void computeVelocitiesParallel();

    /**
     * @brief Exchange velocities at boundaries after each time step
     *
     */
    void exchangeVelocities();

private:
    std::shared_ptr<Communicator> communicator_;                                 //!< comminucator instance
    std::shared_ptr<Partitioning> partitioning_;                                 //!< partitioning instance
    std::unique_ptr<OutputWriterParaviewParallel> outputWriterParaviewParallel_; //!< outputWriterParaviewParallel instance
};

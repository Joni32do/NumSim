#pragma once

#include "discretization/discretization.h"
#include "discretization/donor_cell.h"
#include "discretization/central_differences.h"

#include "solver/pressure_solver.h"
#include "solver/sor.h"
#include "solver/gauss_seidel.h"

#include "output_writer/output_writer_paraview.h"
#include "output_writer/output_writer_text.h"
#include "settings_parser/settings.h"

#include <cmath>
#include <algorithm>
#include <iostream>

/**
 * @class Computation
 * @brief Computation of Simulation 
 * 
 * Combines discretization, pressure solver and outputwriter 
*/
class Computation{
    public:
        /**
         * @brief Initialize the simulation with settings from .txt file
         * 
         * @param argc counts of CLI commands 
         * @param argv values of CLI commands, expect settings.txt filename 
         */
        void initialize(int argc, char *argv[]);

        /**
         * @brief run the whole simulation until t_end
         * 
         */
        void runSimulation();

    private:
        /**
         * @brief Compute the time step width dt from maximum velocities
         * 
         * Uses CFL condition
         */
        void computeTimeStepWidth(double currentTime);


        /**
         * @brief Set boundary values of u and v to correct values
         */
        void applyBoundaryValues();


        /**
         * @brief Compute the preliminary velocities F and G
         */
        void computePreliminaryVelocities();


        /**
         * @brief Compute the right hand side of the pressure equation
         */
        void computeRightHandSide();


        /**
         * @brief Solve the Poisson equation for the pressure
         */
        void computePressure();


        /**
         * @brief Compute the new velocities, u, v, from the preliminary 
         *        velocities F, G and the pressure.
         */
        void computeVelocities();


        Settings settings_;
        std::shared_ptr<Discretization> discretization_;    //!< discretization instance
        std::unique_ptr<PressureSolver> pressureSolver_;    //!< pressureSolver instance
        std::unique_ptr<OutputWriterParaview> outputWriterParaview_;    //!< outputWriterParaview instance
        std::unique_ptr<OutputWriterText> outputWriterText_;    //!< outputWriterText instance
        std::array<double, 2> meshWidth_;   //!< mesh width of domain in x and y direction 
        double dt_; //!< iteration time step

};

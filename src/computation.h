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

class Computation{
    public:
        void initialize(int argc, char *argv[]);

        /**
         * @brief run the whole simulation until t_end
         * 
         */
        void runSimulation();

    private:
        /**
         * @brief compute the time step width dt from maximum velocities
         * 
         * CFL condition
         * 
         */
        void computeTimeStepWidth(double currentTime);


        /**
         * @brief set boundary values of u and v to correct values
         * 
         */
        void applyBoundaryValues();



        /**
         * @brief compute the preliminary velocities F and G
         * 
         */
        void computePreliminaryVelocities();


        /**
         * @brief compute the right hand side of the pressure equation
         * 
         */
        void computeRightHandSide();


        /**
         * @brief solve the Poisson equation for the pressure
         * 
         */
        void computePressure();


        /**
         * @brief compute the new velocities, u, v, from the preliminary velocities F, G and the pressure
         * 
         */
        void computeVelocities();


        Settings settings_;
        std::shared_ptr<Discretization> discretization_;
        std::unique_ptr<PressureSolver> pressureSolver_;
        std::unique_ptr<OutputWriterParaview> outputWriterParaview_;
        std::unique_ptr<OutputWriterText> outputWriterText_;
        std::array<double, 2> meshWidth_;
        double dt_;

};

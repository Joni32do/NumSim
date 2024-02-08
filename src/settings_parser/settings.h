#pragma once

#include <iostream>
#include <array>
#include <fstream>
#include <iostream>
#include <functional>

/**
 * @struct Settings
 * @brief All settings that parametrize a simulation run.
 */
struct Settings
{
  std::array<int, 2> nCells;          //!< number of cells in x and y direction
  std::array<double, 2> physicalSize; //!< physical size of the domain
  double re = 1000;                   //!< reynolds number
  double endTime = 10.0;              //!< end time of the simulation
  double tau = 0.5;                   //!< safety factor for time step width
  double maximumDt = 0.1;             //!< maximum time step width

  std::array<double, 2> g{0., 0.}; //!< external forces

  bool useDonorCell = false; //!< if the donor cell scheme schould be used
  double alpha = 0.5;        //!< factor for donor-cell scheme

  // Boundary Conditions Type
  std::string BCTop = "NoSlip";
  std::string BCRight = "NoSlip";
  std::string BCBottom = "NoSlip";
  std::string BCLeft = "NoSlip";

  // NoSlip boundary conditions
  std::array<double, 2> NoSlipVelTop;    //!< prescribed values of u,v at top of domain
  std::array<double, 2> NoSlipVelRight;  //!< prescribed values of u,v at right of domain
  std::array<double, 2> NoSlipVelBottom; //!< prescribed values of u,v at bottom of domain
  std::array<double, 2> NoSlipVelLeft;   //!< prescribed values of u,v at left of domain

  // Pressure boundary conditions
  double PressureTop;    //!< prescribed pressure BC top of domain
  double PressureRight;  //!< prescribed pressure BC right of domain
  double PressureBottom; //!< prescribed pressure BC bottom of domain
  double PressureLeft;   //!< prescribed pressure BC left of domain

  // Create rectangular obstacle in domain
  bool createRectangularObject = false; //!< wheter a rectangular object in domain should be created

  // Create domain from PNGBitmap
  bool createDomainfromBitmap = false; //!< wheter domain should be created from bitmap 
  std::string pathToBitmap; //!< path to bitmap 

  std::array<double, 2> obstaclePosition; //!< x and y position of rectangular object
  std::array<double, 2> obstacleDimension; //!< x and y dimension of rectangular object

  std::string pressureSolver = "SOR";  //!< which pressure solver to use, "GaussSeidel" or "SOR"
  double omega = 1.0;                  //!< overrelaxation factor
  double epsilon = 1e-5;               //!< tolerance for the residual in the pressure solver
  int maximumNumberOfIterations = 1e5; //!< maximum number of iterations in the solver


  // Fluid Tracer
  bool useFluidTracer = false; //!< Enables Open Surface Evolution
  int particlePerCell = 100; //!< Initializes with given virtual particles per cell
  /**
   * `fixed` -> `n` virtual particles is enough for fluid cell, standard is 1
   * `calculated` -> aims at a constant amount of fluid cells and calculates the threshold accordingly
   */
  std::string fluidTracerMethod = "fixed";
  int fluidTracerThreshold = 1; //!< Threshold for fluidTracerMethod `fixed`
  bool useParticleSource = false; //!< Enables virtual particles source
  std::array<double, 2> particleSource = {0.0, 0.0}; //!< Source of virtual particles

  double printInterval = 0.05; //!< generates output VTK every `printInterval` seconds 

  

  // Writer Options

  /**
   * @brief Parse a text file with settings.
   *
   * Each line contains "<parameterName> = <value>"
   */
  void loadFromFile(std::string filename);

  /**
   * @brief Print all setting to terminal
   */
  void printSettings();

  /**
   * @struct LineContent
   * @brief Contains the content of a single line in a file
   */
  struct LineContent
  {
    std::string parameterName; //!< Name of parameter
    std::string value;         //!< value of the parameter
    bool isContent;            //!< whether line is readable parameter or comment
  };

  /**
   * @brief Reads one line of the settings file
   *
   * @param line line of file to read
   */
  LineContent readSingleLine(std::string line);

  // sets the parameter to the corresponding value
  /**
   * @brief Sets the parameter to the corresponding value
   *
   * @param parameterName paramter name
   * @param value of parameter
   */
  void setParameter(std::string parameterName, std::string value);
};
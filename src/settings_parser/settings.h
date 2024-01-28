#pragma once

#include <iostream>
#include <array>
#include <fstream>
#include <iostream>
#include <functional>
#include "../png_reader/lodepng.h"

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

  bool useBitmap = false;                        //!< if a PNG image will be provided describing the domain
  std::string imagePath = "../input/domain.png"; //!< path for PNG image
  bool useDonorCell = false;                     //!< if the donor cell scheme should be used
  double alpha = 0.5;                            //!< factor for donor-cell scheme

  std::array<double, 2> dirichletBcBottom; //!< prescribed values of u,v at bottom of domain
  std::array<double, 2> dirichletBcTop;    //!< prescribed values of u,v at top of domain
  std::array<double, 2> dirichletBcLeft;   //!< prescribed values of u,v at left of domain
  std::array<double, 2> dirichletBcRight;  //!< prescribed values of u,v at right of domain

  std::string pressureSolver = "SOR";  //!< which pressure solver to use, "GaussSeidel" or "SOR"
  double omega = 1.0;                  //!< overrelaxation factor
  double epsilon = 1e-5;               //!< tolerance for the residual in the pressure solver
  int maximumNumberOfIterations = 1e5; //!< maximum number of iterations in the solver

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
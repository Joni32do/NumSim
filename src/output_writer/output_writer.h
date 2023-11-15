#pragma once

#include "../discretization/discretization.h"

#include <memory>
#include <iostream>

/**
 * @class OutputWriter
 * @brief Inteface class for writing simulation data output.
*/
class OutputWriter
{
public:
  /**
   * @brief Constructor. 
   * 
   * @param discretization discretization object containing data to be written to file
  */ 
  OutputWriter(std::shared_ptr<Discretization> discretization);

  /**
   * @brief Write current velocities to file. 
   * 
   * Filename is output_<fileNo_>.vti
   * 
   * @param currentTime current time of simulation
  */ 
  virtual void writeFile(double currentTime) = 0;

protected:

  std::shared_ptr<Discretization> discretization_;  //!< shared pointer, discretization object containing data to be written to file
  int fileNo_;   //!< a counter that increments for every file written to disk
};

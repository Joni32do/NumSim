#pragma once

#include "output_writer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>


/**
 * @class OutputWriterText
 * @brief Write *.txt files that are useful for debugging.
 * 
 * All values are written to the file as they are stored in the field variables,
 * no interpolation takes place.
*/
class OutputWriterText : 
  public OutputWriter
{
public:
  /**
   * @brief Default constructor. 
  */ 
  using OutputWriter::OutputWriter;

  /**
   * @brief Write current velocities to file. 
   * 
   * Filename is output_<count>.vti
   * 
   * @param currentTime current time of simulation
  */ 
  void writeFile(double currentTime);

  /**
   * @brief Write only current values of pressure to file 
   * 
   * Filename is pressure_<count>.txt
  */ 
  void writePressureFile();

};

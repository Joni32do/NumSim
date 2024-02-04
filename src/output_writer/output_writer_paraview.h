#pragma once

#include "output_writer.h"

#include <vtkSmartPointer.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkImageData.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <cmath>

#include <memory>
#include <iostream>
#include "../boundary/mask.h"

/**
 * @class OutputWriterParaview
 * @brief Write *.vti files that can be viewed with ParaView.
 *
 * The mesh that can be visualized in ParaView corresponds to the mesh of the computational domain.
 * All values are given for the nodes of the mesh, i.e., the corners of each cell.
 * This means, values will be interpolated because the values are stored at positions given by the staggered grid.
 */
class OutputWriterParaview : public OutputWriter
{
public:
  /**
   * @brief Constructor.
   *
   * @param discretization shared pointer to the discretization object that will contain all the data to be written to the file
   */
  OutputWriterParaview(std::shared_ptr<Discretization> discretization, std::shared_ptr<Mask> mask);

  /**
   * @brief Write current velocities to file, filename is output_<count>.vti
   *
   * @param currentTime current time in simulation
   */
  void writeFile(double currentTime);

private:
  vtkSmartPointer<vtkXMLImageDataWriter> vtkWriter_; //!< vtk writer to write ImageData
  std::shared_ptr<Mask> mask_;
};

#include "output_writer/write_paraview_output.h"
#include "settings_parser/settings.h"
#include "storage/array2D.h"
#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>


int main(int argc, char *argv[])
{
  #ifndef NDEBUG
      // only run this code in debug target
      std::cout << "lots of inefficient but informative output . . ." << std::endl;
  #endif

  // if the number of given command line arguments is
  // only 1 (= the program name), print out usage information and exit
  if (argc == 1){
      std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
      return EXIT_FAILURE;
  }

  
  // read in the first argument
  std::string filename = argv[1];

  // print message
  std::cout << "Filename: \"" << filename << "\"" << std::endl;

  // create Settings object
  std::shared_ptr<Settings> settings = std::make_shared<Settings>();

  // load settings from file
  settings->loadFromFile(filename);

  // display all settings on console
  settings->printSettings();

  // write 5 output files
  // for (int i = 0; i < 5; i++)
  // {
  //  writeParaviewOutput(i);
  // }


  return EXIT_SUCCESS;
}
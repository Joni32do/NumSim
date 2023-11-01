#include "output_writer/write_paraview_output.h"
#include "settings_parser/settings.h"
#include "storage/array2D.h"
#include <iostream>
#include <cstdlib>
#include <memory>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
  #ifndef NDEBUG
      // only run this code in debug target
      cout << "lots of inefficient but informative output . . ." << endl;
  #endif

  // if the number of given command line arguments is
  // only 1 (= the program name), print out usage information and exit
  if (argc == 1){
      cout << "usage: " << argv[0] << " <filename>" << endl;
      return EXIT_FAILURE;
  }

  
  // read in the first argument
  string filename = argv[1];

  // print message
  cout << "Filename: \"" << filename << "\"" << endl;

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

  Array2D testarr({8, 4});

  testarr(2,3) = 5.45;

  std::cout << "Hier :::" << "\n";
  std::cout << testarr(2,3) << std::endl;
  std::cout << testarr(1,1) << std::endl;
  std::cout << testarr(10,10) << std::endl;




  return EXIT_SUCCESS;
}
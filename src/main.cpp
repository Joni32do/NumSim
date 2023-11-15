#include "computation.h"
#include <chrono>



int main(int argc, char *argv[])
{
  #ifndef NDEBUG
      // only run this code in debug target
  #endif

  // if the number of given command line arguments is
  // only 1 (= the program name), print out usage information and exit
  if (argc == 1){
      std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
      return EXIT_FAILURE;
  }

  Computation computation;

  // runs the simulation for all arguments and time steps

  // get current system time

  auto start = std::chrono::system_clock::now();
  computation.initialize(argc, argv);
  computation.runSimulation();
  auto end = std::chrono::system_clock::now() - start;
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end).count();
  // #ifndef NDEBUG
  std::cout << "Simulation finished in " << duration << "ms" << std::endl;
  // #endif
  

  return EXIT_SUCCESS;
}
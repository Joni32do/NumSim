#include "computation.h"
#include <chrono>

int main(int argc, char *argv[])
{

  // if the number of given command line arguments is
  // only 1 (= the program name), print out usage information and exit
  if (argc == 1)
  {
    std::cout << "usage: " << argv[0] << " <filename>" << std::endl;
    return EXIT_FAILURE;
  }

  Computation computation;

  auto start = std::chrono::system_clock::now();

  computation.initialize(argc, argv);
  computation.runSimulation();

#ifndef NDEBUG
  auto end = std::chrono::system_clock::now() - start;
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end).count();
  std::cout << "Simulation finished in " << duration << "ms" << std::endl;
#endif

  return EXIT_SUCCESS;
}
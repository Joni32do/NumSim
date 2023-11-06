#include "computation.h"



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
  computation.initialize(argc, argv);

  return EXIT_SUCCESS;
}
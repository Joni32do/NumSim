#include <gtest/gtest.h>
#include "../src/computation.h"


// TEST(Computation, Initialize){
//     Computation comp;
//     char* argv[] = {"test", "../input/settings.txt"};
//     comp.initialize(2, argv);
// }


// std::cout << fluidTracer_->getNumberOfParticles() << std::endl;
//         int particleInDomain = 0;
//         int fluidCells = 0;
//         for (int i = 0; i < fluidTracer_->getNumberOfParticles(); i++)
//         {
//             particleInDomain += fluidTracer_->getParticlePosition(i)[0] > 1.9 
//                              && fluidTracer_->getParticlePosition(i)[0] < settings_.physicalSize[0] 
//                              && fluidTracer_->getParticlePosition(i)[1] > 0.0 
//                              && fluidTracer_->getParticlePosition(i)[1] < settings_.physicalSize[1];
//         }
//         std::cout << particleInDomain << std::endl;
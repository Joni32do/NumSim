#include "pressure_solver.h"

PressureSolver(std::shared_ptr<Discretization> discretization, double epsilon, int maximumNumberOfIterations) : discretization_(discretization), epsilon_(epsilon), maximumNumberOfIterations_(maximumNumberOfIterations) {
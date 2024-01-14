#include "sor.h"
#include <iostream>

SOR::SOR(const std::shared_ptr<Discretization> &data,
         Settings settings) : PressureSolver(data, settings)
{
}

void SOR::solve()
{
    setBoundaryValues();
    double p_x, p_y;
    int n = 0;
    double res = settings_.epsilon + 1;

    double d_fac = (dx2 * dy2) / (2 * (dx2 + dy2));
    do
    {
        for (int i = i_beg; i < i_end; i++)
        {
            for (int j = j_beg; j < j_end; j++)
            {

                p_x = 1 / dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                p_y = 1 / dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1));

                discretization_->p(i, j) = (1 - settings_.omega) * discretization_->p(i, j) + settings_.omega * (d_fac * (p_x + p_y - discretization_->rhs(i, j)));
            }
        }
        setBoundaryValues();
        // Compute the residual with new values
        res = calculateResiduum();
        n++;
    } while (n < settings_.maximumNumberOfIterations && res > settings_.epsilon);

#ifndef NDEBUG
    std::cout << "[Solver] Number of iterations: " << n << ", final residuum: " << res << std::endl;
#endif
}

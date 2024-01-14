#include "pressure_solver.h"


PressureSolver::PressureSolver(std::shared_ptr<Discretization> discretization,
                               Settings settings) : discretization_(discretization),
                                                                settings_(settings)
{
    assert(settings_.epsilon > 0);
    assert(settings_.maximumNumberOfIterations > 0);

    // loop boundaries
    i_beg = discretization_->rhsIBegin();
    i_end = discretization_->rhsIEnd();
    j_beg = discretization_->rhsJBegin();
    j_end = discretization_->rhsJEnd();

    // squared mesh widths
    dx2 = pow(discretization_->dx(), 2);
    dy2 = pow(discretization_->dy(), 2);
}

void PressureSolver::setBoundaryValues()
{
    if (settings_.useNoSlipConditions){
        // Horizontal (without corners)
        for (int i = i_beg; i < i_end; i++)
        {
            discretization_->p(i, j_beg - 1) = discretization_->p(i, j_beg);
            discretization_->p(i, j_end) = discretization_->p(i, j_end - 1);
        }

        // Vertical (without corners)
        for (int j = j_beg - 1; j < j_end + 1; j++)
        {
            discretization_->p(i_beg - 1, j) = discretization_->p(i_beg, j);
            discretization_->p(i_end, j) = discretization_->p(i_end - 1, j);
        }
    } else if (settings_.usePressureConditions){
        
        // Top
        if (settings_.dirTopPressure){
            for (int i = i_beg; i < i_end; i++)
            {
                discretization_->p(i, j_end) = 2 * settings_.dirTopPressure - discretization_->p(i, j_end - 1);
            }
        } else {
            for (int i = i_beg; i < i_end; i++)
            {
                discretization_->p(i, j_end) = discretization_->p(i, j_end - 1);
            }
        }

        // Bottom
        if (settings_.dirBotPressure){
            for (int i = i_beg; i < i_end; i++)
            {
                discretization_->p(i, j_beg-1) = 2 * settings_.dirTopPressure - discretization_->p(i, j_beg);
            }
        } else {
            for (int i = i_beg; i < i_end; i++)
            {
                discretization_->p(i, j_beg - 1) = discretization_->p(i, j_beg);
            }
        }

        // Left
        if (settings_.dirLeftPressure){
            for (int j = j_beg - 1; j < j_end + 1; j++)
            {
                discretization_->p(i_beg - 1, j) = 2 * settings_.dirLeftPressure - discretization_->p(i_beg, j);

            }
        } else {
            for (int j = j_beg - 1; j < j_end + 1; j++)
            {
                discretization_->p(i_beg - 1, j) = discretization_->p(i_beg, j);
            }
        }

        // Right
        if (settings_.dirRightPressure){
            for (int j = j_beg - 1; j < j_end + 1; j++)
            {
                discretization_->p(i_end, j) = 2 * settings_.dirRightPressure - discretization_->p(i_end - 1, j);
            }
        } else {
            for (int j = j_beg - 1; j < j_end + 1; j++)
            {
                discretization_->p(i_end, j) = discretization_->p(i_end - 1, j);
            }
        }
    }
}

double PressureSolver::calculateResiduum()
{
    // 2nd derivative of p in x, y
    double pxx, pyy{0};
    // residuum in a single point, to be added to sum of squares
    double res_current_point{0};
    // to be applied in square root to yield internal product
    double sum_of_squares{0};

    // number of points in rhs grid
    int N = (j_end - j_beg) * (i_end - i_beg);

    for (int i = i_beg; i < i_end; i++)
    {
        for (int j = j_beg; j < j_end; j++)
        {
            pxx = (discretization_->p(i - 1, j) - 2 * discretization_->p(i, j) + discretization_->p(i + 1, j)) / dx2;
            pyy = (discretization_->p(i, j - 1) - 2 * discretization_->p(i, j) + discretization_->p(i, j + 1)) / dy2;
            res_current_point = pxx + pyy - discretization_->rhs(i, j);
            sum_of_squares += pow(res_current_point, 2);
        }
    }
    return sqrt(sum_of_squares / N);
}
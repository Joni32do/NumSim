#include "computation.h"

void Computation::initialize(int argc, char *argv[])
{

    std::string filename = argv[1];

    // load settings from file
    settings_.loadFromFile(filename);
#ifndef NDEBUG
    settings_.printSettings();
#endif

    std::array<double, 2> meshWidth_ = {settings_.physicalSize[0] / settings_.nCells[0],
                                        settings_.physicalSize[1] / settings_.nCells[1]};

    if (settings_.useDonorCell)
    {
        discretization_ = std::make_shared<DonorCell>(settings_.nCells, meshWidth_, settings_.alpha);
    }
    else
    {
        discretization_ = std::make_shared<CentralDifferences>(settings_.nCells, meshWidth_);
    }

    if (settings_.pressureSolver == "SOR")
    {
        pressureSolver_ = std::make_unique<SOR>(discretization_,
                                                settings_.epsilon,
                                                settings_.maximumNumberOfIterations,
                                                settings_.omega);
    }
    else
    {
        pressureSolver_ = std::make_unique<GaussSeidel>(discretization_,
                                                        settings_.epsilon,
                                                        settings_.maximumNumberOfIterations);
    }

    // outputWriterParaview_ = std::make_unique<OutputWriterParaview>(discretization_);
    // outputWriterText_ = std::make_unique<OutputWriterText>(discretization_);
}

void Computation::runSimulation()
{
    double currentTime = 0.;
    do
    {
        applyBoundaryValues();
        computeTimeStepWidth(currentTime);
        computePreliminaryVelocities();
        computeRightHandSide();
        computePressure();
        computeVelocities();

        currentTime += dt_;
        outputWriterParaview_->writeFile(currentTime);

#ifndef NDEBUG
        outputWriterText_->writeFile(currentTime);
        outputWriterText_->writePressureFile();
        std::cout << currentTime << std::endl;
#endif

    } while (currentTime < settings_.endTime);
}

void Computation::applyBoundaryValues()
{

    // set Dirichlet BC

    // BV for u
    int i_beg = discretization_->uIBegin();
    int i_end = discretization_->uIEnd();
    int j_beg = discretization_->uJBegin();
    int j_end = discretization_->uJEnd();

    // Vertical
    for (int j = j_beg; j < j_end; j++)
    {
        discretization_->u(i_beg, j) = settings_.dirichletBcLeft[0];
        discretization_->u(i_end - 1, j) = settings_.dirichletBcRight[0];
    }
    // Horizontal (leave out corners)
    for (int i = i_beg + 1; i < i_end - 1; i++)
    {
        discretization_->u(i, j_beg) = 2 * settings_.dirichletBcBottom[0] - discretization_->u(i, j_beg + 1);
        discretization_->u(i, j_end - 1) = 2 * settings_.dirichletBcTop[0] - discretization_->u(i, j_end - 2);
    }

    // BV for v
    i_beg = discretization_->vIBegin();
    i_end = discretization_->vIEnd();
    j_beg = discretization_->vJBegin();
    j_end = discretization_->vJEnd();

    // Vertical
    for (int j = j_beg; j < j_end; j++)
    {
        discretization_->v(i_beg, j) = 2 * settings_.dirichletBcLeft[1] - discretization_->v(i_beg + 1, j);
        discretization_->v(i_end - 1, j) = 2 * settings_.dirichletBcRight[1] - discretization_->v(i_end - 2, j);
    }
    // Horizontal (leave out corners)
    for (int i = i_beg + 1; i < i_end - 1; i++)
    {
        discretization_->v(i, j_beg) = settings_.dirichletBcBottom[1];
        discretization_->v(i, j_end - 1) = settings_.dirichletBcTop[1];
    }
}

void Computation::computeTimeStepWidth(double currentTime)
{
    // Diffusion operator (always > 0)
    double dx2 = discretization_->dx() * discretization_->dx();
    double dy2 = discretization_->dy() * discretization_->dy();
    double diff = settings_.re / 2 * (dx2 * dy2) / (dx2 + dy2);

    // convection operator restriction u
    double max_u = discretization_->dx() / discretization_->u().findAbsMax();

    // convection operator restriction v
    double max_v = discretization_->dy() / discretization_->v().findAbsMax();

    dt_ = settings_.tau * std::min({diff, max_u, max_v, settings_.maximumDt});


    if (currentTime + dt_ > settings_.endTime){
        dt_ = settings_.endTime - currentTime;
    }

}

void Computation::computePreliminaryVelocities()
{
    // ****************************************
    // Compute F
    // ****************************************
    int f_i_beg = discretization_->fIBegin();
    int f_i_end = discretization_->fIEnd();
    int f_j_beg = discretization_->fJBegin();
    int f_j_end = discretization_->fJEnd();

    // Boundary of F

    // Vertical
    for (int j = f_j_beg - 1; j < f_j_end + 1; j++)
    {
        discretization_->f(f_i_beg, j) = discretization_->u(f_i_beg, j);
        discretization_->f(f_i_end - 1, j) = discretization_->u(f_i_end - 1, j);
    }

    // Horizontal
    for (int i = f_i_beg; i < f_i_end + 1; i++)
    {
        discretization_->f(i, f_j_beg - 1) = discretization_->u(i, f_j_beg - 1);
        // std::cout << discretization_->u(i, f_j_beg) << std::endl;
        discretization_->f(i, f_j_end) = discretization_->u(i, f_j_end);
    }

    // Interior
    for (int i = f_i_beg + 1; i < f_i_end - 1; i++)
    {
        for (int j = f_j_beg; j < f_j_end; j++)
        {

            double diffusion = 1 / settings_.re * (discretization_->computeD2uDx2(i, j) + discretization_->computeD2uDy2(i, j));
            double convection = -discretization_->computeDu2Dx(i, j) - discretization_->computeDuvDy(i, j);
            discretization_->f(i, j) = discretization_->u(i, j) + dt_ * (diffusion + convection + settings_.g[0]);
        }
    }

    // ****************************************
    // Compute G
    // ****************************************

    int g_i_beg = discretization_->gIBegin();
    int g_i_end = discretization_->gIEnd();
    int g_j_beg = discretization_->gJBegin();
    int g_j_end = discretization_->gJEnd();

    // Boundary of G

    // Horizontal
    for (int i = g_i_beg - 1; i < g_i_end + 1; i++)
    {
        discretization_->g(i, g_j_beg) = discretization_->v(i, g_j_beg);
        discretization_->g(i, g_j_end - 1) = discretization_->v(i, g_j_end - 1);
    }

    // Vertical
    for (int j = g_j_beg; j < g_j_end + 1; j++)
    {
        discretization_->g(g_i_beg - 1, j) = discretization_->v(g_i_beg - 1, j);
        discretization_->g(g_i_end, j) = discretization_->v(g_i_end, j);
    }

    // Interior
    for (int i = g_i_beg; i < g_i_end; i++)
    {
        for (int j = g_j_beg + 1; j < g_j_end - 1; j++)
        {

            double diffusion = 1 / settings_.re * (discretization_->computeD2vDx2(i, j) + discretization_->computeD2vDy2(i, j));
            double convection = -discretization_->computeDv2Dy(i, j) - discretization_->computeDuvDx(i, j);

            discretization_->g(i, j) = discretization_->v(i, j) + dt_ * (diffusion + convection + settings_.g[1]);
        }
    }
}

void Computation::computeRightHandSide()
{

    int i_beg = discretization_->rhsIBegin();
    int i_end = discretization_->rhsIEnd();
    int j_beg = discretization_->rhsJBegin();
    int j_end = discretization_->rhsJEnd();

    // Interior
    for (int i = i_beg; i < i_end; i++)
    {
        for (int j = j_beg; j < j_end; j++)
        {
            double dF = (1 / discretization_->dx()) * (discretization_->f(i, j) - discretization_->f(i - 1, j));
            double dG = (1 / discretization_->dy()) * (discretization_->g(i, j) - discretization_->g(i, j - 1));
            discretization_->rhs(i, j) = (1 / dt_) * (dF + dG);
        }
    }
}

void Computation::computePressure()
{
    pressureSolver_->solve();
}

void Computation::computeVelocities()
{

    for (int i = discretization_->uIBegin() + 1; i < discretization_->uIEnd() - 1; i++)
    {
        for (int j = discretization_->uJBegin() + 1; j < discretization_->uJEnd() - 1; j++)
        {
            discretization_->u(i, j) = discretization_->f(i, j) - dt_ * discretization_->computeDpDx(i, j);
        }
    }

    for (int i = discretization_->vIBegin() + 1; i < discretization_->vIEnd() - 1; i++)
    {
        for (int j = discretization_->vJBegin() + 1; j < discretization_->vJEnd() - 1; j++)
        {
            discretization_->v(i, j) = discretization_->g(i, j) - dt_ * discretization_->computeDpDy(i, j);
        }
    }
}

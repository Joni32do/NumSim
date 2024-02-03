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

    mask_ = std::make_shared<Mask>(settings_);

    std::vector<double> traceX = {1.8};
    std::vector<double> traceY = {1.8};

    boundary_ = std::make_shared<Boundary>(mask_, discretization_, settings_);

    if (settings_.pressureSolver == "SOR")
    {
        pressureSolver_ = std::make_unique<SOR>(discretization_,
                                                settings_.epsilon,
                                                settings_.maximumNumberOfIterations,
                                                boundary_,
                                                settings_.omega);
    }
    else
    {
        pressureSolver_ = std::make_unique<GaussSeidel>(discretization_,
                                                        settings_.epsilon,
                                                        settings_.maximumNumberOfIterations,
                                                        boundary_);
    }

    outputWriterParaview_ = std::make_unique<OutputWriterParaview>(discretization_);
    outputWriterText_ = std::make_unique<OutputWriterText>(discretization_);
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
    boundary_->setVelocityBoundaryValues();
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

    if (currentTime + dt_ > settings_.endTime)
    {
        dt_ = settings_.endTime - currentTime;
    }
}

void Computation::computePreliminaryVelocities()
{
    // Compute F
    for (int i = discretization_->fIBegin(); i < discretization_->fIEnd(); i++)
    {
        for (int j = discretization_->fJBegin(); j < discretization_->fJEnd(); j++)
        {
            if (mask_->isFluid(i, j))
            {
                double diffusion = 1 / settings_.re * (discretization_->computeD2uDx2(i, j) + discretization_->computeD2uDy2(i, j));
                double convection = -discretization_->computeDu2Dx(i, j) - discretization_->computeDuvDy(i, j);
                discretization_->f(i, j) = discretization_->u(i, j) + dt_ * (diffusion + convection + settings_.g[0]);
            }
        }
    }

    // Compute G
    for (int i = discretization_->gIBegin(); i < discretization_->gIEnd(); i++)
    {
        for (int j = discretization_->gJBegin(); j < discretization_->gJEnd(); j++)
        {
            if (mask_->isFluid(i, j))
            {
            double diffusion = 1 / settings_.re * (discretization_->computeD2vDx2(i, j) + discretization_->computeD2vDy2(i, j));
            double convection = -discretization_->computeDv2Dy(i, j) - discretization_->computeDuvDx(i, j);
            discretization_->g(i, j) = discretization_->v(i, j) + dt_ * (diffusion + convection + settings_.g[1]);
            }
        }
    }
}

void Computation::computeRightHandSide()
{
    for (int i = discretization_->rhsIBegin(); i < discretization_->rhsIEnd(); i++)
    {
        for (int j = discretization_->rhsJBegin(); j < discretization_->rhsJEnd(); j++)
        {   
            if (mask_->isFluid(i, j))
            {
            double dF = (1 / discretization_->dx()) * (discretization_->f(i, j) - discretization_->f(i - 1, j));
            double dG = (1 / discretization_->dy()) * (discretization_->g(i, j) - discretization_->g(i, j - 1));
            discretization_->rhs(i, j) = (1 / dt_) * (dF + dG);
            }
        }
    }
}

void Computation::computePressure()
{
    pressureSolver_->solve();
}

void Computation::computeVelocities()
{
    // Compute u
    for (int i = discretization_->uIBegin(); i < discretization_->uIEnd(); i++)
    {
        for (int j = discretization_->uJBegin(); j < discretization_->uJEnd(); j++)
        {
            if (mask_->isFluid(i, j))
                discretization_->u(i, j) = discretization_->f(i, j) - dt_ * discretization_->computeDpDx(i, j);
        }
    }

    // Compute v
    for (int i = discretization_->vIBegin() + 1; i < discretization_->vIEnd() - 1; i++)
    {
        for (int j = discretization_->vJBegin() + 1; j < discretization_->vJEnd() - 1; j++)
        {
            if (mask_->isFluid(i, j))
            discretization_->v(i, j) = discretization_->g(i, j) - dt_ * discretization_->computeDpDy(i, j);
        }
    }
}

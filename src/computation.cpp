#include "computation.h"

#include "surface/fluid_tracer.h"
#include <unistd.h>

void Computation::initialize(int argc, char *argv[])
{

    std::string filename = argv[1];

    // load settings from file
    settings_.loadFromFile(filename);
#ifndef NDEBUG
    settings_.printSettings();
#endif

    // create boundary and tracer
    mask_ = std::make_shared<Mask>(settings_);

    if (settings_.createDomainfromBitmap){
        settings_.nCells = {mask_->size()[0]-2, mask_->size()[1]-2};
        settings_.physicalSize = {settings_.physicalSize[0], settings_.physicalSize[0] * settings_.nCells[1] / settings_.nCells[0]};
    }

    meshWidth_ = {settings_.physicalSize[0] / settings_.nCells[0], settings_.physicalSize[1] / settings_.nCells[1]};

    // TODO: remove prints
    std::cout << "Pyhsical size: " << settings_.physicalSize[0] << " " << settings_.physicalSize[1] << std::endl;
    std::cout << "Meshwidth: " << meshWidth_[0] << " " << meshWidth_[1] << std::endl;
    usleep(1000000);

    if (settings_.useDonorCell)
    {
        discretization_ = std::make_shared<DonorCell>(settings_.nCells, meshWidth_, settings_.alpha);
    }
    else
    {
        discretization_ = std::make_shared<CentralDifferences>(settings_.nCells, meshWidth_);
    }
    // Boundary    
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

    // Fluid Tracker
    if (settings_.useFluidTracer)
    {
        fluidTracer_ = std::make_shared<FluidTracer>(settings_.particlePerCell, discretization_, mask_);
    }


    // Output
    outputWriterParaview_ = std::make_unique<OutputWriterParaview>(discretization_, mask_, fluidTracer_);
    outputWriterText_ = std::make_unique<OutputWriterText>(discretization_);
}

void Computation::runSimulation()
{
    // setting
    double printInterval = 0.05;

    // initialize
    double currentTime = 0.;
    double printTime = 0.;
    int stepsBetweenPrints = 0;

    boundary_->setVelocityBoundaryValues();
    boundary_->setPressureSurfaceBC();
    do
    {
        // fluidTracer_->createParticles(0.1, 1.4);

        // mask_->printMask();
        // usleep(100000000);
        // std::cout << "\033[2J\033[1;1H";

        computeTimeStepWidth(currentTime);
        computePreliminaryVelocities();
        computeRightHandSide();
        computePressure();
        computeVelocities();
        
        if (settings_.useFluidTracer)
        {
            // if (settings_.useParticleSource) {
            //     fluidTracer_->createParticles(settings_.particleSource[0], settings_.particleSource[1]);
            // }
            boundary_->setVelocityBoundaryValues(dt_);
            currentTime += dt_;
            computeTimeStepWidth(currentTime);
            fluidTracer_->moveParticles(dt_);
        }
        boundary_->updateBoundary();
        boundary_->setVelocityBoundaryValues();
        boundary_->setPressureSurfaceBC();



        // currentTime += dt_;
        if (currentTime > printTime) {
            outputWriterParaview_->writeFile(currentTime);
            printTime += printInterval;
            std::cout << currentTime << " -> write file, took steps: " << stepsBetweenPrints << std::endl;
            stepsBetweenPrints = 0;
        }

        stepsBetweenPrints++;

#ifndef NDEBUG
        outputWriterText_->writeFile(currentTime);
        outputWriterText_->writePressureFile();
        std::cout << currentTime << std::endl;
        // usleep(100000000);
#endif

    } while (currentTime < settings_.endTime);
}

void Computation::applyBoundaryValues()
{
    // set Dirichlet BC
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
    // F
    for (int i = discretization_->fIBegin(); i < discretization_->fIEnd(); i++)
    {
        for (int j = discretization_->fJBegin(); j < discretization_->fJEnd(); j++)
        {
            if (boundary_->doCalculateF(i, j))
            {
                double diffusion = 1 / settings_.re * (discretization_->computeD2uDx2(i, j) + discretization_->computeD2uDy2(i, j));
                double convection = -discretization_->computeDu2Dx(i, j) - discretization_->computeDuvDy(i, j);
                discretization_->f(i, j) = discretization_->u(i, j) + dt_ * (diffusion + convection + settings_.g[0]);   
            }
        }
    }

    // G
    for (int i = discretization_->gIBegin(); i < discretization_->gIEnd(); i++)
    {
        for (int j = discretization_->gJBegin(); j < discretization_->gJEnd(); j++)
        {
            if (boundary_->doCalculateG(i, j))
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
            if (mask_->isInnerFluid(i, j))
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
    for (int i = discretization_->uIBegin() + 1; i < discretization_->uIEnd() - 1; i++)
    {
        for (int j = discretization_->uJBegin() + 1; j < discretization_->uJEnd() - 1; j++)
        {
            if (boundary_->doCalculateF(i, j))
                discretization_->u(i, j) = discretization_->f(i, j) - dt_ * discretization_->computeDpDx(i, j);
        }
    }

    for (int i = discretization_->vIBegin() + 1; i < discretization_->vIEnd() - 1; i++)
    {
        for (int j = discretization_->vJBegin() + 1; j < discretization_->vJEnd() - 1; j++)
        {
            if (boundary_->doCalculateG(i, j)){
                discretization_->v(i, j) = discretization_->g(i, j) - dt_ * discretization_->computeDpDy(i, j);
            }
        }
    }
}

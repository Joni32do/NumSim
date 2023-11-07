#include "computation.h"

/**
 * @brief 
 * 
 * @param argc 
 * @param argv first is filename
 */
void Computation::initialize(int argc, char *argv[]){

  std::string filename = argv[1];


  // load settings from file
  settings_.loadFromFile(filename);

  // TODO: Logger with settings

  //   settings.printSettings();

  std::array<double, 2> meshWidth_ = {settings_.physicalSize[0] / settings_.nCells[0],
                                      settings_.physicalSize[1] / settings_.nCells[1]};
  
  discretization_ = std::make_shared<Discretization>(settings_.nCells, meshWidth_);
  pressureSolver_ = std::make_unique<PressureSolver>(discretization_,
                                                     settings_.epsilon,
                                                     settings_.maximumNumberOfIterations);
  outputWriterParaview_ = std::make_unique<OutputWriterParaview>(discretization_);
  outputWriterText_ = std::make_unique<OutputWriterText>(discretization_);
  
  // TODO:
  dt_ = 0.1;
}

void Computation::runSimulation(){
    double currentTime = 0.;
    do{
        computeTimeStepWidth();
        applyBoundaryValues();
        computePreliminaryVelocities();
        computeRightHandSide();
        computePressure();
        computeVelocities();

        currentTime += dt_;
        outputWriterParaview_->writeFile(currentTime);
        outputWriterText_->writeFile(currentTime);
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
    for(int j = j_beg + 1; j <= j_end - 1; j++){
        discretization_->u(i_beg, j) = settings_.dirichletBcLeft[0];
        discretization_->u(i_end, j) = settings_.dirichletBcRight[0];
    }
    // Horizontal
    for(int i = i_beg + 1; i <= i_end - 1; i++){
        discretization_->u(i, j_beg) = 2 * settings_.dirichletBcBottom[0] - discretization_->u(i, j_beg + 1);
        discretization_->u(i, j_end) = 2 * settings_.dirichletBcTop[0] - discretization_->u(i, j_end - 1);
    }

    // BV for v
    i_beg = discretization_->vIBegin();
    i_end = discretization_->vIEnd();
    j_beg = discretization_->vJBegin();
    j_end = discretization_->vJEnd();

    // Vertical
    for(int j = j_beg + 1; j <= j_end - 1; j++){
        discretization_->v(i_beg, j) = 2 * settings_.dirichletBcLeft[1] - discretization_->v(i_beg + 1, j);
        discretization_->v(i_end, j) = 2 * settings_.dirichletBcRight[1] - discretization_->v(i_end - 1, j);
    }
    // Horizontal
    for(int i = i_beg + 1; i <= i_end - 1; i++){
        discretization_->v(i, j_beg) = settings_.dirichletBcBottom[1] ;
        discretization_->v(i, j_end) = settings_.dirichletBcTop[1] ;
    }

}



void Computation::computeTimeStepWidth()
{
    // Diffusion operator
    double dx2 = discretization_->dx() * discretization_->dx();
    double dy2 = discretization_->dy() * discretization_->dy();
    double diff = settings_.re/2 * (dx2*dx2)/(dx2+dy2);

    // convection operator restriction u 
    double max_u = discretization_->dx() / discretization_->u().findAbsMax();
    
    // convection operator restriction v
    double max_v = discretization_->dy() / discretization_->v().findAbsMax();

    
    // dt_ = std::min(diff, max_u, max_v, max_dt)

    double min_uv = std::min(max_u, max_v);
    double min = std::min(min_uv, diff);
    double min_dt = std::min(min, settings_.maximumDt)

    dt_ = settings_.tau * min_dt;
}


void Computation::computePreliminaryVelocities()
{
    // Compute F
    int i_beg = discretization_->uIBegin();
    int i_end = discretization_->uIEnd();
    int j_beg = discretization_->uJBegin();
    int j_end = discretization_->uJEnd();


    // Vertical
    for(int j = j_beg + 1; j <= j_end - 1; j++){
        discretization_->f(i_beg, j) = discretization_->u(i_beg, j);
        discretization_->f(i_end, j) = discretization_->u(i_end, j);
    }

    // Horizontal
    for(int i = i_beg + 1; i <= i_end - 1; i++){
        discretization_->f(i, j_beg) = discretization_->u(i, j_beg);
        discretization_->f(i, j_end) = discretization_->u(i, j_end);
    }


    // CInterior
    for(int i = i_beg + 1; i <= i_end - 1; i++){
        for(int j = j_beg + 1 ; j <= j_end - 1; j++){

            double diffusion = 1/settings_.re * (discretization_->computeD2uDx2(i, j) 
                                               + discretization_->computeD2uDy2(i, j));
            double convection = - discretization_->computeDu2Dx(i,j) 
                                - discretization_->computeDuvDy(i, j);

            discretization_->f(i, j) = discretization_->u(i, j) 
                                        + dt_ * (diffusion + convection + settings_.g[0]);
        }
    }

    // Compute G

    i_beg = discretization_->vIBegin();
    i_end = discretization_->vIEnd();
    j_beg = discretization_->vJBegin();
    j_end = discretization_->vJEnd();

    // Vertical
    for(int j = j_beg + 1; j <= j_end - 1; j++){
        discretization_->g(i_beg, j) = discretization_->v(i_beg, j);
        discretization_->g(i_end, j) = discretization_->v(i_end, j);
    }

    // Horizontal
    for(int i = i_beg + 1; i <= i_end - 1; i++){
        discretization_->g(i, j_beg) = discretization_->v(i, j_beg);
        discretization_->g(i, j_end) = discretization_->v(i, j_end);
    }


    // Interior
    for(int i = i_beg + 1; i <= i_end - 1; i++){
        for(int j = j_beg + 1 ; j <= j_end - 1; j++){

            double diffusion = 1/settings_.re * (discretization_->computeD2vDx2(i, j) 
                                               + discretization_->computeD2vDy2(i, j));
            double convection = - discretization_->computeDv2Dy(i,j) 
                                - discretization_->computeDuvDx(i, j);
                                
            discretization_->f(i, j) = discretization_->v(i, j) 
                                        + dt_ * (diffusion + convection + settings_.g[1]);
        }
    }
}


void Computation::computeRightHandSide(){

    int i_beg = discretization_->pIBegin();
    int i_end = discretization_->pIEnd();
    int j_beg = discretization_->pJBegin();
    int j_end = discretization_->pJEnd();

    // Interior
    for(int i = i_beg + 1; i <= i_end - 1; i++){
        for(int j = j_beg + 1 ; j <= j_end - 1; j++){
            double dF = 1/discretization_-> dx() * (discretization_->f(i, j) - discretization_->f(i-1, j));
            double dG = 1/discretization_-> dy() * (discretization_->g(i, j) - discretization_->g(i, j-1));
            discretization_->rhs(i, j) = 1/dt_ * (dF + dG);
        }
    }
}

void Computation::computePressure(){
    pressureSolver_->solve();
}


void Computation::computeVelocities(){

}



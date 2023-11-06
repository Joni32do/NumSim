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
  
  
  dt_ = 0.1;
}


void Computation::computeTimeStepWidth():
{

    // settings_.tau
    // settings_.maximumDt
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
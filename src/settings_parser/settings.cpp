#include <fstream> // for file operations
#include <iostream> // for cout
#include "settings.h"

void Settings::loadFromFile(std::string filename){
    
    // open file
    std::ifstream file(filename.c_str(), std::ios::in);

    // check if file is open
    if (!file.is_open()){
        std::cout << "Could not open parameter file \"" << filename << "\"." << std::endl;
        return ;
    }

    // loop over lines of file
    for (int lineNo = 0;; lineNo++){

        // read line
        std::string line;
        getline(file, line);

        // at the end of the file, break the "for" loop
        if (file.eof())
            break;

        // remove whitespace at beginning of line, if there is any
        if(line.find_first_not_of(" \t") == std::string::npos) // line only contains whitespace
            continue; // skip line
        else
            line.erase(0, line.find_first_not_of(" \t")); // remove initial whitespace

        // if first character is a '#', skip line
        if(line[0] == '#')
            continue;

        // if line does not contain a '=' sign, skip line
        if(line.find_first_of("=") == std::string::npos)
            continue;
        
        // parse parameter name
        //! second argument of substr is the number of characters in new str
        std::string parameterName = line.substr(0, line.find_first_of(" =\t"));
        //! second argument of erase is the number of characters to erase
        //! '=' is also to be erased
        line.erase(0, line.find_first_of("=") + 1);

        // parse value
        // new variable for the sake of readability
        std::string value = line;

        // remove whitespace at beginning of value
        if(value.find_first_not_of(" \t") != 0)
            value.erase(0, value.find_first_not_of(" \t"));

        // remove comments and whitespace at end of value
        // the value std::npos in the second argument of subtr
        // indicates all characters until the end of the string,
        // thus no 'if' clause is needed
        value = value.substr(0, value.find_first_of(" #\t"));

        // parse actual value and set corresponding parameter
        // problem description
        if (parameterName == "physicalSizeX")
            Settings::physicalSize[0] = atof(value.c_str());
        else if(parameterName == "physicalSizeY")
            Settings::physicalSize[1] = atof(value.c_str());
        else if(parameterName == "endTime")
            Settings::endTime = atof(value.c_str());
        else if(parameterName == "re")
            Settings::re = atof(value.c_str());
        else if(parameterName == "gX")
            Settings::g[0] = atof(value.c_str());
        else if(parameterName == "gY")
            Settings::g[1] = atof(value.c_str());
            
        // Dirichlet boundary conditions
        else if(parameterName == "dirichletBottomX")
            Settings::dirichletBcBottom[0] = atof(value.c_str());
        else if(parameterName == "dirichletBottomY")
            Settings::dirichletBcBottom[1] = atof(value.c_str());
        else if(parameterName == "dirichletTopX")
            Settings::dirichletBcTop[0] = atof(value.c_str());
        else if(parameterName == "dirichletTopY")
            Settings::dirichletBcTop[1] = atof(value.c_str());
        else if(parameterName == "dirichletLeftX")
            Settings::dirichletBcLeft[0] = atof(value.c_str());
        else if(parameterName == "dirichletLeftY")
            Settings::dirichletBcLeft[1] = atof(value.c_str());
        else if(parameterName == "dirichletRightX")
            Settings::dirichletBcRight[0] = atof(value.c_str());
        else if(parameterName == "dirichletRightY")
            Settings::dirichletBcRight[1] = atof(value.c_str());

        //Discretization parameters
        else if(parameterName == "nCellsX")
            Settings::nCells[0] = atoi(value.c_str());
        else if(parameterName == "nCellsY")
            Settings::nCells[1] = atoi(value.c_str());
        else if(parameterName == "useDonorCell")
            {
                if(value == "true" || value == "True")
                    Settings::useDonorCell = true;
                else if(value == "false" || value == "False")
                    Settings::useDonorCell = false;
                else
                    throw std::invalid_argument("useDonorCell must be a boolean (true or false).");
            }
        else if(parameterName == "alpha")
            Settings::alpha = atof(value.c_str());
        else if(parameterName == "tau")
            Settings::tau = atof(value.c_str());
        else if(parameterName == "maximumDt")
            Settings::maximumDt = atof(value.c_str());

        // Solver parameters
        else if(parameterName == "pressureSolver")
            {
                if(value == "SOR" || value == "GaussSeidel" || value == "CG")
                    Settings::pressureSolver = value;
                else
                    throw std::invalid_argument("Supported values for pressureSolver are SOR, CG and GaussSeidel.");
            }
        else if(parameterName == "omega")
            Settings::omega = atof(value.c_str());
        else if(parameterName == "epsilon")
            Settings::epsilon = atof(value.c_str());
        else if(parameterName == "maximumNumberOfIterations")
            Settings::maximumNumberOfIterations = atof(value.c_str());

    }
}

void Settings::printSettings(){
  std::cout << "Settings: " << std::endl
    << "  physicalSize: " << physicalSize[0] << " x " << physicalSize[1] << ", nCells: " << nCells[0] << " x " << nCells[1] << std::endl
    << "  endTime: " << endTime << " s, re: " << re << ", g: (" << g[0] << "," << g[1] << "), tau: " << tau << ", maximum dt: " << maximumDt << std::endl
    << "  dirichletBC: bottom: (" << dirichletBcBottom[0] << "," << dirichletBcBottom[1]  << ")"
    << ", top: ("  << dirichletBcTop[0] << "," << dirichletBcTop[1]  << ")"
    << ", left: ("  << dirichletBcLeft[0] << "," << dirichletBcLeft[1] << ")"
    << ", right: ("  << dirichletBcRight[0] << "," << dirichletBcRight[1] << ")" << std::endl
    << "  useDonorCell: " << std::boolalpha << useDonorCell << ", alpha: " << alpha << std::endl
    << "  pressureSolver: " << pressureSolver << ", omega: " << omega << ", epsilon: " << epsilon << ", maximumNumberOfIterations: " << maximumNumberOfIterations << std::endl;
}
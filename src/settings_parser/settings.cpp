#include "settings.h"

void Settings::loadFromFile(std::string filename)
{

    // open file
    std::ifstream file(filename.c_str(), std::ios::in);

    // check if file is open
    if (!file.is_open())
    {
        std::cout << "Could not open parameter file \"" << filename << "\"." << std::endl;
        return;
    }

    // loop over lines of file
    for (int lineNo = 0;; lineNo++)
    {

        // read line
        std::string line;
        getline(file, line);

        // at the end of the file, break the "for" loop
        if (file.eof())
            break;

        LineContent lineContent = Settings::readSingleLine(line);

        if (!lineContent.isContent)
        {
            continue;
        }

        // parse actual value and set corresponding parameter
        Settings::setParameter(lineContent.parameterName, lineContent.value);
    }
}

void Settings::printSettings()
{
    std::cout << "Settings: " << std::endl
              << "  physicalSize: " << physicalSize[0] << " x " << physicalSize[1] << ", nCells: " << nCells[0] << " x " << nCells[1] << std::endl
              << "  endTime: " << endTime << " s, re: " << re << ", g: (" << g[0] << "," << g[1] << "), tau: " << tau << ", maximum dt: " << maximumDt << std::endl
              << "  dirichletBC: bottom: (" << NoSlipVelBottom[0] << "," << NoSlipVelBottom[1] << ")"
              << ", top: (" << NoSlipVelTop[0] << "," << NoSlipVelTop[1] << ")"
              << ", left: (" << NoSlipVelLeft[0] << "," << NoSlipVelLeft[1] << ")"
              << ", right: (" << NoSlipVelRight[0] << "," << NoSlipVelRight[1] << ")" << std::endl
              << "  useDonorCell: " << std::boolalpha << useDonorCell << ", alpha: " << alpha << std::endl
              << "  pressureSolver: " << pressureSolver << ", omega: " << omega << ", epsilon: " << epsilon << ", maximumNumberOfIterations: " << maximumNumberOfIterations << std::endl;
}

Settings::LineContent Settings::readSingleLine(std::string line)
{
    Settings::LineContent lineContent;
    lineContent.isContent = false;

    // find first character that is not a whitespace
    int idx_first_non_white = line.find_first_not_of(" \t");
    int idx_equalSign = line.find_first_of("=");

    // Checks if line contains content

    // if first character is a '#', skip line
    if (line[0] == '#')
        return lineContent;

    // if line only contains whitespace, skip line
    // line only contains whitespace
    if (idx_first_non_white == std::string::npos)
        return lineContent;

    // if line does not contain a '=' sign, skip line
    if (idx_equalSign == std::string::npos)
        return lineContent;

    // --- Line contains content ---
    // devide content on '=' and assign `parameterName` and `value` without whitespaces
    lineContent.isContent = true;

    // 2nd argument of substr is the number of characters in new str
    line.erase(0, idx_first_non_white);
    lineContent.parameterName = line.substr(0, line.find_first_of(" =\t"));

    // 2nd argument of erase is the number of characters to erase
    line.erase(0, line.find_first_of("=") + 1);

    // remove whitespace at beginning of value
    if (line.find_first_not_of(" \t") != 0)
        line.erase(0, line.find_first_not_of(" \t"));

    // remove comments and whitespace at end of value
    // if `line.find_first_of() == std::npos` line is not changed
    lineContent.value = line.substr(0, line.find_first_of(" #\t"));

    return lineContent;
}

void Settings::setParameter(std::string parameterName, std::string value)
{
    if (parameterName == "physicalSizeX")
        Settings::physicalSize[0] = atof(value.c_str());
    else if (parameterName == "physicalSizeY")
        Settings::physicalSize[1] = atof(value.c_str());
    else if (parameterName == "endTime")
        Settings::endTime = atof(value.c_str());
    else if (parameterName == "re")
        Settings::re = atof(value.c_str());
    else if (parameterName == "gX")
        Settings::g[0] = atof(value.c_str());
    else if (parameterName == "gY")
        Settings::g[1] = atof(value.c_str());

    // // Boundary conditions type
    else if (parameterName == "BCTop"){
        if (value == "NoSlip" || value == "Pressure")
                Settings::BCTop = value;
            else
                throw std::invalid_argument("Supported values for BCTop are NoSlip and Pressure.");
    }
    else if (parameterName == "BCRight"){
        if (value == "NoSlip" || value == "Pressure")
                Settings::BCRight = value;
            else
                throw std::invalid_argument("Supported values for BCRight are NoSlip and Pressure.");
    }
    else if (parameterName == "BCBottom"){
        if (value == "NoSlip" || value == "Pressure")
                Settings::BCBottom = value;
            else
                throw std::invalid_argument("Supported values for BCBottom are NoSlip and Pressure.");
    }
    else if (parameterName == "BCLeft"){
        if (value == "NoSlip" || value == "Pressure")
                Settings::BCLeft = value;
            else
                throw std::invalid_argument("Supported values for BCLeft are NoSlip and Pressure.");
    }

    // NoSlip boundary conditions
    else if (parameterName == "NoSlipVelTopX" || parameterName == "dirichletTopX")
        Settings::NoSlipVelTop[0] = atof(value.c_str());
    else if (parameterName == "NoSlipVelTopY" || parameterName == "dirichtelTopY")
        Settings::NoSlipVelTop[1] = atof(value.c_str());
    else if (parameterName == "NoSlipVelRightX"  || parameterName == "dirichtelRightX")
        Settings::NoSlipVelRight[0] = atof(value.c_str());
    else if (parameterName == "NoSlipVelRightY"  || parameterName == "dirichtelRightY")
        Settings::NoSlipVelRight[1] = atof(value.c_str());
    else if (parameterName == "NoSlipVelBottomX"  || parameterName == "dirichtelBottomX")
        Settings::NoSlipVelBottom[0] = atof(value.c_str());
    else if (parameterName == "NoSlipVelBottomY"  || parameterName == "dirichtelBottomY")
        Settings::NoSlipVelBottom[1] = atof(value.c_str());
    else if (parameterName == "NoSlipVelLeftX"  || parameterName == "dirichtelLeftX")
        Settings::NoSlipVelLeft[0] = atof(value.c_str());
    else if (parameterName == "NoSlipVelLeftY"  || parameterName == "dirichtelLeftY")
        Settings::NoSlipVelLeft[1] = atof(value.c_str());

    // Pressure BoundaryConditions. 
    else if (parameterName == "PressureTop")
        Settings::PressureTop = atof(value.c_str());
    else if (parameterName == "PressureRight")
        Settings::PressureRight = atof(value.c_str());
    else if (parameterName == "PressureBottom")
        Settings::PressureBottom = atof(value.c_str());
    else if (parameterName == "PressureLeft")
        Settings::PressureLeft = atof(value.c_str());

    
    // Rectangular object in domain
    else if (parameterName == "createRectangularObject")
    {
        if (value == "true" || value == "True")
            Settings::createRectangularObject = true;
        else if (value == "false" || value == "False")
            Settings::createRectangularObject = false;
        else
            throw std::invalid_argument("createRectangularObject must be a boolean (true or false).");
    }
    else if (parameterName == "obstaclePositionX"){
        Settings::obstaclePosition[0] = atof(value.c_str());
    }
    else if (parameterName == "obstaclePositionY")
        Settings::obstaclePosition[1] = atof(value.c_str());
    else if (parameterName == "obstacleDimensionX")
        Settings::obstacleDimension[0] = atof(value.c_str());    
    else if (parameterName == "obstacleDimensionY")
        Settings::obstacleDimension[1] = atof(value.c_str());

    // Domain from Bitmap
    else if (parameterName == "createDomainfromBitmap")
    {
        if (value == "true" || value == "True")
            Settings::createDomainfromBitmap = true;
        else if (value == "false" || value == "False")
            Settings::createDomainfromBitmap = false;
        else
            throw std::invalid_argument("createDomainfromBitmap must be a boolean (true or false).");
    }
    else if (parameterName == "pathToBitmap")
        Settings::pathToBitmap = value;
    
    // Discretization parameters
    else if (parameterName == "nCellsX")
        Settings::nCells[0] = atoi(value.c_str());
    else if (parameterName == "nCellsY")
        Settings::nCells[1] = atoi(value.c_str());
    else if (parameterName == "useDonorCell")
    {
        if (value == "true" || value == "True")
            Settings::useDonorCell = true;
        else if (value == "false" || value == "False")
            Settings::useDonorCell = false;
        else
            throw std::invalid_argument("useDonorCell must be a boolean (true or false).");
    }
    else if (parameterName == "alpha")
        Settings::alpha = atof(value.c_str());
    else if (parameterName == "tau")
        Settings::tau = atof(value.c_str());
    else if (parameterName == "maximumDt")
        Settings::maximumDt = atof(value.c_str());

    // Solver parameters
    else if (parameterName == "pressureSolver")
    {
        if (value == "SOR" || value == "GaussSeidel" || value == "CG")
            Settings::pressureSolver = value;
        else
            throw std::invalid_argument("Supported values for pressureSolver are SOR, CG and GaussSeidel.");
    }
    else if (parameterName == "omega")
        Settings::omega = atof(value.c_str());
    else if (parameterName == "epsilon")
        Settings::epsilon = atof(value.c_str());
    else if (parameterName == "maximumNumberOfIterations")
        Settings::maximumNumberOfIterations = atof(value.c_str());


    else if(parameterName == "useFluidTracer")
    {
        if (value == "true" || value == "True")
            Settings::useFluidTracer = true;
        else if (value == "false" || value == "False")
            Settings::useFluidTracer = false;
        else
            throw std::invalid_argument("createDomainfromBitmap must be a boolean (true or false)."); 
    }
    else if(parameterName == "particlePerCell")
        particlePerCell = atof(value.c_str());
    else if(parameterName == "fluidTracerMethod")
        if (value == "single" || value == "threshold")
            Settings::fluidTracerMethod = value;
        else
            throw std::invalid_argument("Supported values for fluidTracerMethod are single and threshold");
}

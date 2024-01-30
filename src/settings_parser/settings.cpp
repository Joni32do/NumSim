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
              << "  physicalSize: " << physicalSize[0] << " x " << physicalSize[1] << std::endl
              << "  useBitmap: " << std::boolalpha << useBitmap << ", nCells: " << nCells[0] << " x " << nCells[1] << std::endl
              << "  endTime: " << endTime << " s, re: " << re << ", g: (" << g[0] << "," << g[1] << "), tau: " << tau << ", maximum dt: " << maximumDt << std::endl
              << "  dirichletBC: bottom: (" << dirichletBcBottom[0] << "," << dirichletBcBottom[1] << ")"
              << ", top: (" << dirichletBcTop[0] << "," << dirichletBcTop[1] << ")"
              << ", left: (" << dirichletBcLeft[0] << "," << dirichletBcLeft[1] << ")"
              << ", right: (" << dirichletBcRight[0] << "," << dirichletBcRight[1] << ")" << std::endl
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

    // Dirichlet boundary conditions
    else if (parameterName == "dirichletBottomX")
        Settings::dirichletBcBottom[0] = atof(value.c_str());
    else if (parameterName == "dirichletBottomY")
        Settings::dirichletBcBottom[1] = atof(value.c_str());
    else if (parameterName == "dirichletTopX")
        Settings::dirichletBcTop[0] = atof(value.c_str());
    else if (parameterName == "dirichletTopY")
        Settings::dirichletBcTop[1] = atof(value.c_str());
    else if (parameterName == "dirichletLeftX")
        Settings::dirichletBcLeft[0] = atof(value.c_str());
    else if (parameterName == "dirichletLeftY")
        Settings::dirichletBcLeft[1] = atof(value.c_str());
    else if (parameterName == "dirichletRightX")
        Settings::dirichletBcRight[0] = atof(value.c_str());
    else if (parameterName == "dirichletRightY")
        Settings::dirichletBcRight[1] = atof(value.c_str());

    // Discretization parameters
    else if (parameterName == "useBitmap"){
        if (value == "true" || value == "True")
            Settings::useBitmap = true;
        else if (value == "false" || value == "False")
            Settings::useBitmap = false;
        else
            throw std::invalid_argument("useBitmap must be a boolean (true or false).");
    }
    else if (parameterName == "imagePath"){
        if (Settings::useBitmap == true){
            Settings::imagePath == value;

            std::vector<unsigned char> img; // The raw pixels
            unsigned width, height;

            // Decode the PNG file
            unsigned error = lodepng::decode(img, width, height, value);
            // If there's an error, display it
            if (error) {
                std::cerr << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
                // return 1;
            }
            #ifndef NDEBUG
                else{
                    std::cout << "Successfully read PNG image\n";
                    std::cout << "img size (vector size): " << img.size() << std::endl;
                    unsigned r, g, b;
                    for (int k=0; k<img.size()/4; ++k){
                        r = static_cast<int>(img[4*k]);
                        g = static_cast<int>(img[4*k+1]);
                        b = static_cast<int>(img[4*k+2]);
                        std::cout << k << "th  pixel: " << static_cast<int>(r) << ", " << static_cast<int>(g) << ", " << static_cast<int>(b) << std::endl;
                    }
                }
            #endif

            // Set number of cells from image
            Settings::nCells[0] = width;
            Settings::nCells[1] = height;
            Settings::image = img;
        }
    }
    else if (parameterName == "nCellsX"){
        if (Settings::useBitmap == false)
            Settings::nCells[0] = atoi(value.c_str());
    }
    else if (parameterName == "nCellsY"){
        if (Settings::useBitmap == false)
            Settings::nCells[1] = atoi(value.c_str());
    }
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
}

#include "mask.h"

Mask::Mask(Settings settings) : settings_(settings)
{
  assert(settings_.nCells[0] > 0 && settings_.nCells[1] > 0);
  size_ = {settings_.nCells[0] + 2, settings_.nCells[1] + 2};
  data_.resize(size_[0] * size_[1], FLUID);

  printMask();
  if (settings_.createRectangularObject)
  {
    Mask::makeRectangularObstacle();
  }
  std::cout << "\n\n\n";

  printMask();

  setDomainBoundary();
  
  std::cout << "\n\n\n";

  printMask();
}

int &Mask::operator()(int i, int j)
{
  assert(i >= 0 && i < size_[0] && j >= 0 && j < size_[1]);
  return data_[i + j * size_[0]];
}

int Mask::operator()(int i, int j) const
{
  assert(i >= 0 && i < size_[0] && j >= 0 && j < size_[1]);
  return data_[i + j * size_[0]];
}

std::array<int, 2> Mask::size() const
{
  return size_;
}

void Mask::resetMask()
{
  for (int i = 0; i < size_[0]; i++)
  {
    for (int j = 0; j < size_[1]; j++)
    {
      if (!isObstacle(i, j))
      {
        data_[i + j * size_[0]] = AIR;
      }
    }
  }
}

void Mask::updateMaskBoundaries()
{
  for (int i = 0; i < size_[0]; i++)
  {
    for (int j = 0; j < size_[1]; j++)
    {
      int idx = i + j * size_[0];

      if (Mask::isFluid(i, j))
      {
        data_[idx] = 1 * Mask::isNotAir(i - 1, j) + 2 * Mask::isNotAir(i, j + 1) + 4 * Mask::isNotAir(i + 1, j) + 8 * Mask::isNotAir(i, j - 1);
      }
    }
  }
}

bool Mask::isFluid(int i, int j) const
{
  if (i < 0 || i >= size_[0] || j < 0 || j >= size_[1])
  {
    return false;
  }
  else
  {
    return (data_[i + j * size_[0]] < FLUID_TYPE);
  }
}

bool Mask::isObstacle(int i, int j) const
{
  return (data_[i + j * size_[0]] >= OBSTACLE);
}

bool Mask::isFluidBorder(int i, int j) const
{
  return data_[i + j * size_[0]] < FLUID;
}

bool Mask::isBorder(int i, int j) const
{
  return isFluidBorder(i, j) || isObstacle(i, j);
}

bool Mask::isAir(int i, int j) const
{
  return (data_[i + j * size_[0]] == AIR);
}

bool Mask::isNotAir(int i, int j) const
{
  if (i < 0 || i >= size_[0] || j < 0 || j >= size_[1])
  {
    return true;
  }
  else
  {
    return (data_[i + j * size_[0]] != AIR);
  }
}

int Mask::getNumberOfFluidCells() const
{
  int numFluidCells = 0;
  for (int i = 0; i < size_[0]; i++)
  {
    for (int j = 0; j < size_[1]; j++)
    {
      if (isFluid(i, j))
      {
        numFluidCells++;
      }
    }
  }
  return numFluidCells;
}

// ************************
// P R I M I T I V E S
// ************************

void Mask::makeRectangularObstacle()
{

  double scaleX = settings_.obstaclePosition[0] / settings_.physicalSize[0];
  double scaleY = settings_.obstaclePosition[1] / settings_.physicalSize[1];
  int nCellsX = size_[0] - 2;
  int nCellsY = size_[1] - 2;

  int i_beg = static_cast<int>(std::floor(scaleX * nCellsX)) + 1;
  int i_end = static_cast<int>(std::ceil((settings_.obstaclePosition[0] + settings_.obstacleDimension[0]) / settings_.physicalSize[0] * nCellsX)) + 1;
  int j_beg = static_cast<int>(std::floor(scaleY * nCellsY)) + 1;
  int j_end = static_cast<int>(std::ceil((settings_.obstaclePosition[1] + settings_.obstacleDimension[1]) / settings_.physicalSize[1] * nCellsY)) + 1;

  std::cout << i_beg << " from obstacle Position" << settings_.obstaclePosition[0] << " and size " << size_[0] - 2 << std::endl;
  std::cout << i_end << " from obstacle Size " << settings_.obstacleDimension[0] << " and size " << size_[0] - 2 << std::endl;
  
  for (int i = i_beg; i < i_end; i++)
  {
    for (int j = j_beg; j < j_end; j++)
    {
      data_[i + j * size_[0]] = OBSTACLE;
    }
  }
};

void Mask::setDomainBoundary()
{
  // Set domain boundaries
  
  for (int i = 0; i < size_[0]; i++)
  {
    // Bottom Domain Boundary
    if (data_[i+size_[0]] == OBSTACLE)
      data_[i] = OBSTACLE_INSIDE;
    else if (settings_.BCBottom == "NoSlip")
      data_[i] = DOMAIN_BOTTOM_NOSLIP;
    else if (settings_.BCBottom == "Pressure")
      data_[i] = DOMAIN_BOTTOM_PRESSURE;


    // Top Domain Boundary
    if (data_[i + (size_[1] - 2) * size_[0]] == OBSTACLE)
      data_[i + (size_[1] - 1) * size_[0]] = OBSTACLE_INSIDE;
    else if (settings_.BCTop == "NoSlip")
      data_[i + (size_[1] - 1) * size_[0]] = DOMAIN_TOP_NOSLIP;
    else if (settings_.BCTop == "Pressure")
      data_[i + (size_[1] - 1) * size_[0]] = DOMAIN_TOP_PRESSURE;
  }    

  for (int j = 0; j < size_[1]; j++)
  { 
    // Left Domain Boundary
    if (data_[j * size_[0]+1] == OBSTACLE) 
      data_[j * size_[0]] = OBSTACLE_INSIDE;
    else if (settings_.BCLeft == "NoSlip")
      data_[j * size_[0]] = DOMAIN_LEFT_NOSLIP;
    else if (settings_.BCLeft == "Pressure")
      data_[j * size_[0]] = DOMAIN_LEFT_PRESSURE;

    // Right Domain Boundary
    if (data_[(size_[0] - 2) + j * size_[0]] == OBSTACLE)
      data_[(size_[0] - 1) + j * size_[0]] = OBSTACLE_INSIDE;
    else if (settings_.BCRight == "NoSlip")
      data_[(size_[0] - 1) + j * size_[0]] = DOMAIN_RIGHT_NOSLIP;
    else if (settings_.BCRight == "Pressure")
      data_[(size_[0] - 1) + j * size_[0]] = DOMAIN_RIGHT_PRESSURE;
  }

  data_[0] = OBSTACLE_INSIDE;
  data_[size_[0]-1] = OBSTACLE_INSIDE;
  data_[0+size_[0]*(size_[1]-1)] = OBSTACLE_INSIDE;
  data_[size_[0]-1+size_[0]*(size_[1]-1)] = OBSTACLE_INSIDE;


}

// ******************************
//  P R I N T I N G
// ******************************

void Mask::printMask() const
{
  for (int j = size_[1] - 1; j >= 0; j--)
  {
    for (int i = 0; i < size_[0]; i++)
    {
      if (data_[i + j * size_[0]] == AIR)
      {
        std::cout << "    ";
      }
      else
      {
        std::cout << std::setw(3) << std::setfill('0') << data_[i + j * size_[0]] << " ";
      }
    }
    std::cout << std::endl;
  }
}

// void Mask::createBitmap() const{
//     std::ofstream file("output.bmp", std::ios::binary);
//     int width = size_[0];
//     int height = size_[1];
//     std::vector<int> pixels = data_;

//     // Bitmap header
//     unsigned char header[54] = {
//         0x42, 0x4D, 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0, 40, 0,
//         0, 0, static_cast<unsigned char>(width), static_cast<unsigned char>(width >> 8), static_cast<unsigned char>(width >> 16), static_cast<unsigned char>(width >> 24),
//         static_cast<unsigned char>(height), static_cast<unsigned char>(height >> 8), static_cast<unsigned char>(height >> 16), static_cast<unsigned char>(height >> 24),
//         1, 0, 24, 0
//     };

//     file.write(reinterpret_cast<char>(header), sizeof(header));

//     // Bitmap data
//     int rowSize = (3 width + 3) & ~3;  // Ensure each row is a multiple of 4 bytes
//     unsigned char padding[3] = {0, 0, 0};

//     for (int y = height - 1; y >= 0; --y) {
//         for (int x = 0; x < width; ++x) {
//             unsigned char pixel[3] = {
//                 pixels[y * width + x],
//                 pixels[y * width + x],
//                 pixels[y * width + x]
//             };
//             file.write(reinterpret_cast<char>(pixel), sizeof(pixel));
//         }
//         file.write(reinterpret_cast<char>(padding), rowSize - 3 * width);
//     }

//     file.close();
// }

#include "mask.h"

Mask::Mask(std::array<int, 2> size) : size_({size[0]+2, size[1]+2})
{
  assert(size[0] > 0 && size[1] > 0);
  data_.resize(size_[0] * size_[1], FLUID);

  // Set domain boundaries to obstacle
  for (int i = 0; i < size_[0]; i++)
  {
    data_[i] = DOMAIN_BOTTOM;
    data_[i + (size_[1] - 1) * size_[0]] = DOMAIN_TOP;
  }
  for (int j = 0; j < size_[1]; j++)
  {
    data_[j * size_[0]] = DOMAIN_LEFT;
    data_[(size_[0] - 1) + j * size_[0]] = DOMAIN_RIGHT;
  }
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

void Mask::updateMaskBoundaries()
{
  for (int i = 0; i < size_[0]; i++)
  {
    for (int j = 0; j < size_[1]; j++)
    {
      int idx = i + j * size_[0];

      if (Mask::isFluid(i, j))
      {
        data_[idx] =  1 * Mask::isNotAir(i - 1, j)
                    + 2 * Mask::isNotAir(i, j + 1)
                    + 4 * Mask::isNotAir(i + 1, j)
                    + 8 * Mask::isNotAir(i, j - 1);

      }
    }
  }
}




bool Mask::isFluid(int i, int j) const
    {
      if (i < 0 || i >= size_[0] || j < 0 || j >= size_[1]){
        return false;
      } else {
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
      } else {
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


void Mask::makeRectangularObstacle(std::array<double, 2> obstaclePosition_ , std::array<double, 2> obstacleSize_){
  // only works for global size {1.0; 1.0}
    int i_beg = static_cast<int>(std::floor(obstaclePosition_[0] * size_[0]));
    int i_end = static_cast<int>(std::ceil(obstaclePosition_[0] + obstacleSize_[0]) * size_[0]);
    int j_beg = static_cast<int>(std::floor(obstaclePosition_[1] * size_[1]));
    int j_end = static_cast<int>(std::ceil(obstaclePosition_[1] + obstacleSize_[1]) * size_[1]);
    // interior obstacle
    for (int i = i_beg + 1; i < i_end - 1; i++){
        for (int j = j_beg + 1; j < j_end - 1; j++){
            data_[i + j * size_[0]] = OBSTACLE;
        }
    }
    // boundary obstacle
    for (int i = i_beg; i < i_end; i++){
        data_[i + j_beg * size_[0]] = OBSTACLE_BORDER_BOTTOM;
        data_[i + (j_end - 1) * size_[0]] = OBSTACLE_BORDER_TOP;
    }
    for (int j = j_beg; j < j_end; j++){
        data_[i_beg + j * size_[0]] = OBSTACLE_BORDER_LEFT;
        data_[(i_end - 1) + j * size_[0]] = OBSTACLE_BORDER_RIGHT;
    }  
};






// ******************************
//  P R I N T I N G
// ******************************

void Mask::printMask() const{
    for (int j = size_[1] - 1; j >= 0; j--){
        for (int i = 0; i < size_[0]; i++){
            std::cout << std::setw(3) << std::setfill('0') << data_[i + j * size_[0]] << " ";
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

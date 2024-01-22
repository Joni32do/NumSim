#include "mask.h"

Mask::Mask(std::array<int, 2> size) : size_({size[0]+2, size[1]+2})
{
  assert(size[0] > 0 && size[1] > 0);
  data_.resize(size_[0] * size_[1], FLUID);
    
  // Set domain boundaries to obstacle
  for (int i = 0; i < size_[0]; i++)
  {
    data_[i] = OBSTACLE_BORDER_TOP; // is bottom therefore the top of an obstacle
    data_[i + (size_[1] - 1) * size_[0]] = OBSTACLE_BORDER_BOTTOM;
  }
  for (int j = 0; j < size_[1]; j++)
  {
    data_[j * size_[0]] = OBSTACLE_BORDER_RIGHT;
    data_[(size_[0] - 1) + j * size_[0]] = OBSTACLE_BORDER_LEFT;
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
        int val = data_[i + j * size_[0]];
        return (val >= FLUID_BORDER_LEFT && 
                val <= FLUID_TYPE);
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
///////////////////////////////////////////////////
// Temporary Print Statements

void Mask::printMask() const{
    for (int j = size_[1] - 1; j >= 0; j--){
        for (int i = 0; i < size_[0]; i++){
            std::cout << data_[i + j * size_[0]] << " ";
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

#include "mask.h"

Mask::Mask(std::array<int, 2> size) : size_(size)
{
  assert(size[0] > 0 && size[1] > 0);
  data_.resize(size_[0] * size_[1], 0);
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
  std::vector<int> data_temp = data_;
  for (int i = 0; i < size_[0]; i++)
  {
    for (int j = 0; j < size_[1]; j++)
    {
      int idx = i + j * size_[0];
      bool isFluid = data_[idx] < 100;

      if (isFluid)
      {
        data_temp[idx] =  1 * (data_[idx - 1] < FLUID_TYPE) 
                        + 2 * (data_[idx + size_[0]] < FLUID_TYPE)
                        + 4 * (data_[idx + 1] < FLUID_TYPE)
                        + 8 * (data_[idx - size_[0]] < FLUID_TYPE);

      }
    }
  }
  data_ = data_temp;
}









bool Mask::isFluid(int i, int j) const
    {
        return (data_[i + j * size_[0]] < FLUID_TYPE);
    }

bool Mask::isObstacle(int i, int j) const
    {
        return (data_[i + j * size_[0]] > OBSTACLE);
    }

bool Mask::isFluidBorder(int i, int j) const
    {
        int val = data_[i + j * size_[0]];
        return (val >= FLUID_BORDER_LEFT && 
                val <= FLUID_TYPE);
    }

bool Mask::isAir(int i, int j) const
    {
        return (data_[i + j * size_[0]] == AIR);
    }
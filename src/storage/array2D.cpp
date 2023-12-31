#include "array2D.h"

Array2D::Array2D(std::array<int, 2> size) : size_(size)
{
  assert(size[0] > 0 && size[1] > 0);
  data_.resize(size_[0] * size_[1], 0.0);
}

double &Array2D::operator()(int i, int j)
{
  const int index = j * size_[0] + i;

  // Assert that indices are inside size of array
  assert(0 <= i && i < size_[0]);
  assert(0 <= j && j < size_[1]);
  assert(j * size_[0] + i < (int)data_.size());

  return data_[index];
}

double Array2D::operator()(int i, int j) const
{
  const int index = j * size_[0] + i;

  // Assert that indices are inside size of array
  assert(0 <= i && i < size_[0]);
  assert(0 <= j && j < size_[1]);
  assert(j * size_[0] + i < (int)data_.size());

  return data_[index];
}

std::array<int, 2> Array2D::size() const
{
  return size_;
}

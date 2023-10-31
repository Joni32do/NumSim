#include "array2D.h"
#include <cassert>



Array2D::Array2D(const std::array<int, 2> size): size_(size){
    data_ = std::vector<double>(size[0]*size[1]);
}

//! This overwriters the () operator -> can be used ´arr(i,j) = 1.0´
double &Array2D::operator()(int i, int j)
{
  const int index = j*size_[0] + i;

  // assert that indices are in range
  assert(0 <= i && i < size_[0]);
  assert(0 <= j && j < size_[1]);
  assert(j*size_[0] + i < (int)data_.size());

  return data_[index];
}

double Array2D::operator()(int i, int j) const
{
  const int index = j*size_[0] + i;

  // assert that indices are in range
  assert(0 <= i && i < size_[0]);
  assert(0 <= j && j < size_[1]);
  assert(j*size_[0] + i < (int)data_.size());

  return data_[index];
}

std::array<int,2> Array2D::size() const{
    return size_;
}
    


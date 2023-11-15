#include "array2D.h"


/*! 
 * Allocate data, initialize to 0
 */
Array2D::Array2D(std::array<int, 2> size): size_(size){
  assert(size[0] > 0 && size[1] > 0);
  data_.resize(size_[0]*size_[1], 0.0);
}

/*! 
 * Asserts that indices are in range
 */
double &Array2D::operator()(int i, int j)
{
  const int index = j*size_[0] + i;

  assert(0 <= i && i < size_[0]);
  assert(0 <= j && j < size_[1]);
  assert(j*size_[0] + i < (int)data_.size());

  return data_[index];
}

/*! 
 * Asserts that indices are in range
 */
double Array2D::operator()(int i, int j) const
{
  const int index = j*size_[0] + i;

  assert(0 <= i && i < size_[0]);
  assert(0 <= j && j < size_[1]);
  assert(j*size_[0] + i < (int)data_.size());

  return data_[index];
}

std::array<int,2> Array2D::size() const{
    return size_;
}
    
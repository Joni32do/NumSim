#include "array2D.h"

Array2D::Array2D(std::array<int, 2> size): size_(size){
  // allocate data, initialize to 0
  assert(size[0] > 0 && size[1] > 0);
  data_.resize(size_[0]*size_[1], 0.0);
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

double Array2D::operator()(double i_float, double j_float) const
{
  // The purpose is to enable `u(1.5, 2)` aka half steps

  // assert that indices are in range
  assert(0 <= i_float && i_float < size_[0]);
  assert(0 <= j_float && j_float < size_[1]);

  // compute indices
  int i = (int) i_float;
  int j = (int) j_float;

  // code copy from interpolation:
  double i_weight = i_float - i;
  double j_weight = j_float - j;
  
  // value lower left node
  double down_left = (1-i_weight)*(*this)(i,j);
  double down_right = (i_weight)*(*this)(i+1,j);
  double up_left = (1-i_weight)*(*this)(i,j+1);
  double up_right = (i_weight)*(*this)(i+1,j+1);
  return (1-j_weight)*(down_left + down_right) + j_weight*(up_left + up_right);

}

std::array<int,2> Array2D::size() const{
    return size_;
}
    
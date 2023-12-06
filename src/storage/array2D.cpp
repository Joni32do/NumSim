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

std::vector<double> Array2D::getRow(int row, int start, int end, int step) const{
  int slice_size = int(std::ceil((end - start)/step));
  std::vector<double> slice(slice_size);
  //int ownRankNo_;
  //MPI_Comm_rank(MPI_COMM_WORLD, &ownRankNo_);
  //std::cout << ownRankNo_ <<  "end: " << end << i << " " << j << std::endl;
  int idx = 0;
  // std::cout << "\tInside getRow\n";

  for (int i = start; i < end; i += step){
    // std::cout << "\t\t" << i << std::endl;
    slice[idx] = (*this)(i, row);
    // std::cout << "\t\tafter slice" << std::endl;
    idx++;
    // std::cout << "\t\tindex:" << idx-1 << std::endl;
    // std::cout << "\t\tslice size:" << slice.size() << std::endl;
  }
  // std::cout << "\tsending vector of size: " << slice.size() << std::endl;

  return slice;
}


std::vector<double> Array2D::getColumn(int col, int start, int end, int step) const{
  int slice_size = int(std::ceil((end - start)/step));
  std::vector<double> slice(slice_size);
  int idx = 0;
  // std::cout << "\tInside getColumn\n";
  for (int j = start; j < end; j += step){
    // std::cout << "\t\t" << j << std::endl;
    slice[idx] = (*this)(col, j);
    idx++;
  }
  return slice;
}

void* Array2D::data(){
  return data_.data();
}
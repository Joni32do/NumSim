#include "array2D.h"



Array2D::Array2D(const std::array<int, 2> size): size_(size){
    data_ = std::vector<double>(size[0]*size[1]);
}

// This overwriters the () operator -> can be used ´arr(i,j) = 1.0´
double Array2D::operator()(int i, int j){
    return data_[j*size_[0]+i];
}

double& Array2D::operator()(int i, int j) const{
    return data_[j*size_[0]+i];
}

std::array<int,2> Array2D::size() const{
    return size_;
}
    


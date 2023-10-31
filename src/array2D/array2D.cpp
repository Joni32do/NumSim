#include "array2D.h"



Array2D::Array2D(const std::array<int, 2> size): size_(size){
    data_ = std::vector<double>(size[0]*size[1]);
}


double Array2D::operator()(int i, int j){
    return data_[j*size_[0]+i];
}

double & Array2D::operator()(int i, int j) const{
    const double val = data_[j*size_[0]+i]; 
    return val&;
}

std::array<int,2> Array2D::size() const{
    return size_;
}
    


#include "array2D.h"

Array2D::Array2D(const int n_x, const int n_y): n_x(n_x), n_y(n_y){
    points = std::vector<double>(n_x*n_y);
}

double Array2D::get_point(int i, int j){
    return points[n_x*j+i];
}

void Array2D::set_point(int i, int j, double value){
    points[n_x*j+i] = value;
}
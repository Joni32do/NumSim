#include "discretization.h"

Discretization::Discretization(std::array<int, 2> nCells, std::array<double, 2> meshWidth): StaggeredGrid(nCells, meshWidth){

}

double Discretization::computeDu2Dx(int i, int j) const{
    return (u(i+1,j)*u(i+1,j)-u(i,j)*u(i,j))/dx();
}

//TODO: implement the rest of the derivatives
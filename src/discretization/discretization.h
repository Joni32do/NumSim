#pragma once

#include <array>
#include <vector>
#include "staggered_grid.h"

class Discretization : public StaggeredGrid
{
public:
    Discretization(std::array<int, 2> nCells, std::array<double, 2> meshWidth);
    // 1st derivatives
    virtual double computeDu2Dx(int i, int j) const = 0;
    virtual double computeDv2Dy(int i, int j) const = 0;

    // mixed derivatives for advection
    virtual double computeDuvDx(int i, int j) const = 0;
    virtual double computeDuvDy(int i, int j) const = 0;

    // 2nd derivatives for diffusion
    virtual double computeD2uDx2(int i, int j) const;
    virtual double computeD2uDy2(int i, int j) const;
    virtual double computeD2vDx2(int i, int j) const;
    virtual double computeD2vDy2(int i, int j) const;

    // compute pressure derivatives
    virtual double computeDpDx(int i, int j) const;
    virtual double computeDpDy(int i, int j) const;
};
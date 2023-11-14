#pragma once

#include <array>
#include <vector>
#include <cmath>
#include "discretization.h"

class DonorCell : public Discretization
{
public:
    DonorCell(std::array<int, 2> nCells, std::array<double, 2> meshWidth, double alpha);

    // Additional derivatives for donor cell scheme
    double computeDu2Dx(int i, int j) const override;
    double computeDv2Dy(int i, int j) const override;
    double computeDuvDx(int i, int j) const override;
    double computeDuvDy(int i, int j) const override;

private:
    const double alpha_;
};
#pragma once

#include <array>
#include <vector>
#include "discretization.h"

class CentralDifferences : public Discretization
{
public:
    CentralDifferences(std::array<int, 2> nCells, std::array<double, 2> meshWidth);

    // Additional derivatives for central differences scheme
    double computeDu2Dx(int i, int j) const override;
    double computeDv2Dy(int i, int j) const override;
    double computeDuvDx(int i, int j) const override;
    double computeDuvDy(int i, int j) const override;
};

#pragma once

#include <array>
#include <vector>
#include "discretization.h"

class CentralDifferences : public Discretization
{
public:
    CentralDifferences(std::array<int, 2> nCells, std::array<double, 2> meshWidth);

    // Additional derivatives for central differences scheme
    virtual double computeDu2Dx(int i, int j) const override;
    virtual double computeDv2Dy(int i, int j) const override;
    virtual double computeDuvDx(int i, int j) const override;
    virtual double computeDuvDy(int i, int j) const override;
};

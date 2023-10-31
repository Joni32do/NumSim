#pragma once

#include <array>
#include <vector>
#include "discretization.h"

class DonorCell : public Discretization
{
public:
    DonorCell(std::array<int, 2> nCells, std::array<double, 2> meshWidth, double alpha);

    // Additional derivatives for donor cell scheme
    virtual double computeDu2Dx(int i, int j) const override;
    virtual double computeDv2Dy(int i, int j) const override;
    virtual double computeDuvDx(int i, int j) const override;
    virtual double computeDuvDy(int i, int j) const override;

private:
    const double alpha_;
};
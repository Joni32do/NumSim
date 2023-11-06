#include "central_differences.h"

CentralDifferences::CentralDifferences(std::array<int, 2> nCells, std::array<double, 2> meshWidth):
            Discretization(nCells, meshWidth)
{
}


double CentralDifferences::computeDu2Dx(int i, int j) const
{
    double u_iminus_j = 0.5 * (u(i, j) + u(i - 1, j));
    double u_iplus_j = 0.5 * (u(i + 1, j) + u(i, j));
    return (u_iplus_j * u_iplus_j - u_iminus_j * u_iminus_j) / dx();
    // Shorter Version:
    // return (u(i+0.5,j)*u(i+0.5,j) - u(i-0.5,j)*u(i-0.5,j)) / dx();
}


double CentralDifferences::computeDv2Dy(int i, int j) const
{
    double v_i_jminus = 0.5 * (v(i, j) + v(i, j - 1));
    double v_i_jplus = 0.5 * (v(i, j + 1) + v(i, j));
    return (v_i_jplus * v_i_jplus - v_i_jminus * v_i_jminus) / dy();
}


double CentralDifferences::computeDuvDx(int i, int j) const
{
    // left
    double u_i_jplus = 0.5 * (u(i, j) + u(i, j + 1));
    double v_iplus_j = 0.5 * (v(i + 1, j) + v(i, j));
    // right
    double u_iminus1_jplus = 0.5 * (u(i - 1, j) + u(i - 1, j + 1));
    double v_iminus_j = 0.5 * (v(i - 1, j) + v(i, j));
    return (u_i_jplus * v_iplus_j - u_iminus1_jplus * v_iminus_j) / dx();
}


double CentralDifferences::computeDuvDy(int i, int j) const
{
    double v_iplus_j = 0.5 * (v(i + 1, j) + v(i, j));
    double v_iplus_jminus1 = 0.5 * (v(i + 1, j - 1) + v(i, j - 1));
    double u_i_jplus = 0.5 * (u(i, j) + u(i, j + 1));
    double u_i_jminus = 0.5 * (u(i, j) + u(i, j - 1));
    return (v_iplus_j * u_i_jplus - v_iplus_jminus1 * u_i_jminus) / dy();
}


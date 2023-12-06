#include "donor_cell.h"

DonorCell::DonorCell(std::array<int, 2> nCells, std::array<double, 2> meshWidth, double alpha) : Discretization(nCells, meshWidth),
                                                                                                 alpha_(alpha)
{
}

double DonorCell::computeDuvDx(int i, int j) const
{
    double u_iplus1_jminus = 0.5*(u(i+1,j)+u(i+1,j-1)); // adapted this for parallel
    double u_i_jminus = 0.5*(u(i,j)+u(i,j-1)); // adapted this for parallel
    double v_iplus_j = 0.5 * (v(i, j) + v(i + 1, j));
    double v_ipdiff_j = 0.5 * (v(i, j) - v(i + 1, j));
    double v_iminus_j = 0.5 * (v(i - 1, j) + v(i, j));
    double v_imdiff_j = 0.5 * (v(i - 1, j) - v(i, j));

    double term_1 = u_iplus1_jminus * v_iplus_j - u_i_jminus * v_iminus_j;
    double term_2 = std::abs(u_iplus1_jminus) * v_ipdiff_j - std::abs(u_i_jminus) * v_imdiff_j;
    return (term_1 + alpha_ * term_2) / dx();
}

double DonorCell::computeDuvDy(int i, int j) const
{
    double v_iminus_jplus1 = 0.5*(v(i,j+1)+v(i-1, j+1)); // adapted after staggered grid extension
    double v_iminus_j = 0.5*(v(i,j)+v(i-1, j)); // adapted after staggered grid extension
    double u_i_jplus = 0.5 * (u(i, j) + u(i, j + 1));
    double u_i_jpdiff = 0.5 * (u(i, j) - u(i, j + 1));
    double u_i_jminus = 0.5 * (u(i, j - 1) + u(i, j));
    double u_i_jmdiff = 0.5 * (u(i, j - 1) - u(i, j));

    double term_1 = v_iminus_jplus1 * u_i_jplus - v_iminus_j * u_i_jminus;
    double term_2 = std::abs(v_iminus_jplus1) * u_i_jpdiff - std::abs(v_iminus_j) * u_i_jmdiff;
    return (term_1 + alpha_ * term_2) / dy();
}

double DonorCell::computeDu2Dx(int i, int j) const
{
    // Calculation via abs
    double u_iplus_j = 0.5 * (u(i, j) + u(i + 1, j));
    double u_ipdiff_j = 0.5 * (u(i, j) - u(i + 1, j));
    double u_iminus_j = 0.5 * (u(i - 1, j) + u(i, j));
    double u_imdiff_j = 0.5 * (u(i - 1, j) - u(i, j));

    double term_1 = (u_iplus_j * u_iplus_j - u_iminus_j * u_iminus_j);
    double term_2 = (std::abs(u_iplus_j) * u_ipdiff_j - std::abs(u_iminus_j) * u_imdiff_j);
    return (term_1 + alpha_ * term_2) / dx();
}

double DonorCell::computeDv2Dy(int i, int j) const
{
    // Absolute Value
    double v_i_jplus = 0.5 * (v(i, j) + v(i, j + 1));
    double v_i_jpdiff = 0.5 * (v(i, j) - v(i, j + 1));
    double v_i_jminus = 0.5 * (v(i, j - 1) + v(i, j));
    double v_i_jmdiff = 0.5 * (v(i, j - 1) - v(i, j));

    double term_1 = (v_i_jplus * v_i_jplus - v_i_jminus * v_i_jminus);
    double term_2 = (std::abs(v_i_jplus) * v_i_jpdiff - std::abs(v_i_jminus) * v_i_jmdiff);
    return (term_1 + alpha_ * term_2) / dy();
}
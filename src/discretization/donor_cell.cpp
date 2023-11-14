#include "donor_cell.h"

DonorCell::DonorCell(std::array<int, 2> nCells, std::array<double, 2> meshWidth, double alpha):
            Discretization(nCells, meshWidth),
            alpha_(alpha)
{
}

double DonorCell::computeDuvDx(int i, int j) const
{
    // double u_iminus1_jplus = 0.5 * (u(i - 1, j + 1) + u(i - 1, j)); // u(i-1, j+1/2)
    // double u_i_jplus = 0.5 * (u(i, j + 1) + u(i, j));
    // double left;
    // double right;


    // if(u_i_jplus > 0){
    //     left = u_i_jplus * v(i, j);
    // } else {
    //     left = u_i_jplus * v(i + 1, j);
    // }

    // if (u_iminus1_jplus > 0){
    //     right = u_iminus1_jplus * v(i - 1, j);
    // } else {
    //     right = u_iminus1_jplus * v(i, j);
    // }

    // // donor cell discretization
    // double dc_DuvDx = (left - right) / dx();

    // // original central difference discretization
    // double v_iplus_j = 0.5 * (v(i + 1, j) + v(i, j));
    // double v_iminus_j = 0.5 * (v(i - 1, j) + v(i, j));
    // double cd_DuvDx = (u_i_jplus * v_iplus_j - u_iminus1_jplus * v_iminus_j) / dx();

    // return (1 - alpha_) * cd_DuvDx + alpha_ * dc_DuvDx;

    // Calculation via abs
    double u_i_jplus = 0.5 * (u(i, j + 1) + u(i, j));
    double u_iminus1_jplus = 0.5 * (u(i - 1, j + 1) + u(i - 1, j)); // u(i-1, j+1/2)
    double v_iplus_j =  0.5 * (v(i, j) + v(i + 1, j));
    double v_ipdiff_j = 0.5 * (v(i, j) - v(i + 1, j));
    double v_iminus_j = 0.5 * (v(i - 1, j) + v(i, j));
    double v_imdiff_j = 0.5 * (v(i - 1, j) - v(i, j));

    double term_1 = u_i_jplus * v_iplus_j - u_iminus1_jplus * v_iminus_j;
    double term_2 = std::abs(u_i_jplus) * v_ipdiff_j - std::abs(u_iminus1_jplus) * v_imdiff_j;
    return (term_1 + alpha_ * term_2)/dx();  


}



double DonorCell::computeDuvDy(int i, int j) const
{
    // double v_iplus_j = 0.5 * (v(i + 1, j) + v(i, j));
    // double v_iplus_jminus1 = 0.5 * (v(i + 1, j - 1) + v(i, j - 1));
    // double left;
    // double right;

    // if (v_iplus_j > 0){
    //     left = v_iplus_j * u(i, j);
    // } else {
    //     left = v_iplus_j * u(i, j + 1);
    // }

    // if(v_iplus_jminus1 > 0){
    //     right = v_iplus_jminus1 * u(i, j - 1);
    // } else {
    //     right = v_iplus_jminus1 * u(i, j);
    // }

    // // donor cell discretization
    // double dc_DuvDy = (left - right) / dy();

    // // original central difference discretization
    // double u_i_jplus = 0.5 * (u(i, j) + u(i, j + 1));
    // double u_i_jminus = 0.5 * (u(i, j) + u(i, j - 1));
    // double cd_DuvDy = (v_iplus_j * u_i_jplus - v_iplus_jminus1 * u_i_jminus) / dy();

    // return (1 - alpha_) * cd_DuvDy + alpha_* dc_DuvDy;

    // Calculation via abs
    double v_iplus_j = 0.5 * (v(i + 1, j) + v(i, j));
    double v_iplus_jminus1 = 0.5 * (v(i + 1, j - 1) + v(i, j - 1));
    double u_i_jplus =  0.5 * (u(i, j) + u(i, j + 1));
    double u_i_jpdiff = 0.5 * (u(i, j) - u(i, j + 1));
    double u_i_jminus = 0.5 * (u(i, j - 1) + u(i, j));
    double u_i_jmdiff = 0.5 * (u(i, j - 1) - u(i, j));

    double term_1 = v_iplus_j * u_i_jplus - v_iplus_jminus1 * u_i_jminus;
    double term_2 = std::abs(v_iplus_j) * u_i_jpdiff - std::abs(v_iplus_jminus1) * u_i_jmdiff;
    return (term_1 + alpha_ * term_2)/dy();    
}

double DonorCell::computeDu2Dx(int i, int j) const
{
    // double u_iplus_j = 0.5 * (u(i + 1, j) + u(i, j));
    // double u_iminus_j = 0.5 * (u(i, j) + u(i - 1, j));    
    // double left;
    // double right;

    // if (u_iplus_j > 0){
    //     left = u_iplus_j * u(i, j);
    // } else {
    //     left = u_iplus_j * u(i + 1, j);
    // }

    // if(u_iminus_j > 0){
    //     right = u_iminus_j * u(i - 1, j);
    // } else {
    //     right = u_iminus_j * u(i, j);
    // }

    // // donor cell discretization
    // double dc_Du2Dx = (left - right) / dx();

    // // origina central difference discretization
    // double cd_Du2Dx = (u_iplus_j * u_iplus_j - u_iminus_j * u_iminus_j) / dx();

    // return ((1 - alpha_) * cd_Du2Dx + alpha_ * dc_Du2Dx);

    // Calculation via abs
    double u_iplus_j =  0.5 * (u(i, j) + u(i + 1, j));
    double u_ipdiff_j = 0.5 * (u(i, j) - u(i + 1, j));
    double u_iminus_j = 0.5 * (u(i - 1, j) + u(i, j)); 
    double u_imdiff_j = 0.5 * (u(i - 1, j) - u(i, j)); 

    double term_1 = (u_iplus_j * u_iplus_j - u_iminus_j * u_iminus_j);
    double term_2 = (std::abs(u_iplus_j) * u_ipdiff_j - std::abs(u_iminus_j) * u_imdiff_j);
    return (term_1 + alpha_ * term_2)/dx();
}


double DonorCell::computeDv2Dy(int i, int j) const
{
    // double v_i_jplus = 0.5 * (v(i, j + 1) + v(i, j));
    // double v_i_jminus = 0.5 * (v(i, j) + v(i, j - 1));
    // double left;
    // double right;

    // if(v_i_jminus > 0){
    //     right = v_i_jminus * v(i, j - 1);
    // } else {
    //     right = v_i_jminus * v(i, j);
    // }

    // if (v_i_jplus > 0){
    //     left = v_i_jplus * v(i, j);
    // } else {
    //     left = v_i_jplus * v(i, j + 1);
    // }

    // // donor cell discretization
    // double dc_Dv2Dy = (left - right) / dy();

    // // original central difference discretization
    // double cd_Dv2Dy = (v_i_jplus * v_i_jplus - v_i_jminus * v_i_jminus) / dy();

    // return (1 - alpha_) * cd_Dv2Dy + alpha_ * dc_Dv2Dy;

    // Absolute Value
    double v_i_jplus =  0.5 * (v(i, j) + v(i, j + 1));
    double v_i_jpdiff = 0.5 * (v(i, j) - v(i, j + 1));
    double v_i_jminus = 0.5 * (v(i, j - 1) + v(i, j));
    double v_i_jmdiff = 0.5 * (v(i, j - 1) - v(i, j));


    double term_1 = (v_i_jplus * v_i_jplus - v_i_jminus * v_i_jminus);
    double term_2 = (std::abs(v_i_jplus) * v_i_jpdiff - std::abs(v_i_jminus) * v_i_jmdiff);
    return (term_1 + alpha_ * term_2)/dy();
}
#include <gtest/gtest.h>
#include "../src/discretization/donor_cell.h"
#include <cmath>

// Pressure terms

TEST(DonorCell, ComputeDpDx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};

    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.p(1,1) = 1;
    dc_1.p(2,1) = -1;

    EXPECT_EQ(dc_1.computeDpDx(1, 1), -2.0);
};



TEST(DonorCell, ComputeDpDy){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.p(1,1) = 1;
    dc_1.p(1,2) = -1;

    EXPECT_EQ(dc_1.computeDpDy(1, 1), -2.0);
};

// Diffusion terms

TEST(DonorCell, ComputeD2uDx2){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(1,2) = 1;
    dc_1.u(2,2) = -1;
    dc_1.u(3,2) = 1;

    EXPECT_EQ(dc_1.computeD2uDx2(2, 2), 4.0);
};


TEST(DonorCell, ComputeD2uDy2){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(2,1) = 1;
    dc_1.u(2,2) = -1;
    dc_1.u(2,3) = 1;

    EXPECT_EQ(dc_1.computeD2uDy2(2, 2), 4.0);
};


TEST(DonorCell, ComputeD2vDx2){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.v(1,2) = 1;
    dc_1.v(2,2) = -1;
    dc_1.v(3,2) = 1;

    EXPECT_EQ(dc_1.computeD2vDx2(2, 2), 4.0);
};


TEST(DonorCell, ComputeD2vDy2){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.v(2,1) = 1;
    dc_1.v(2,2) = -1;
    dc_1.v(2,3) = 1;

    EXPECT_EQ(dc_1.computeD2vDy2(2, 2), 4.0);
};

// Convection terms

TEST(DonorCell, ComputeDu2Dx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(1,2) = -1;
    dc_1.u(2,2) = 0;
    dc_1.u(3,2) = 1;

    EXPECT_EQ(dc_1.computeDu2Dx(2, 2), 0.0);
};


TEST(DonorCell, ComputeDv2Dy){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(2,1) = -1;
    dc_1.u(2,2) = 0;
    dc_1.u(2,3) = 1;

    EXPECT_EQ(dc_1.computeDv2Dy(2, 2), 0.0);
};


TEST(DonorCell, ComputeDuvDy){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.u(2,2) = 0;
    
    dc_1.v(2,2) = -1;
    dc_1.v(3,2) = -1;
    // -> v(2.5, 2) = -1
    dc_1.u(2,3) = 1;
    // -> u(2,2.5) = 0.5
    double left = -0.5;

    dc_1.v(2,1) = -1;
    dc_1.v(3,1) = -1;
    // -> v(2.5, 1) = -1
    dc_1.u(2,1) = -1;
    // -> u(2,1.5) = -0.5
    double right = 0.5;
    double expected = (left - right) / (meshWidth[1]);

    EXPECT_EQ(dc_1.computeDuvDy(2, 2), expected);
};


TEST(DonorCell, ComputeDuvDx){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    DonorCell dc_1(n_cells, meshWidth, 0);
    dc_1.v(2,2) = 0;
    
    dc_1.u(2,2) = -1;
    dc_1.u(2,3) = -1;
    // -> u(2, 2.5) = -1
    dc_1.v(3,2) = -1;
    // -> u(2.5,2) = -0.5
    double left = 0.5;

    dc_1.u(1,2) = -1;
    dc_1.u(1,3) = -1;
    // -> u(1, 2.5) = -1
    dc_1.v(1,2) = 1;
    // -> v(1.5,2) = 0.5
    double right = -0.5;

    double expected = (left - right) / (meshWidth[0]);

    EXPECT_EQ(dc_1.computeDuvDx(2, 2), expected);
};



// ===================== Varying alphas

TEST(DonorCell, ComputeDuvDy_VaryingAlpha){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    double const alpha {0.5}; // DonorCell.alpha_ is private
    DonorCell dc(n_cells, meshWidth, alpha);
    int const i {2}, j {2};
    // i, j
    dc.u(i, j) = 0;
    dc.v(i, j) = -1; // -> u(2, 2.5) = 0.5
    // i+1, j
    dc.v(i+1, j) = -1; // -> v(2.5, 2) = -1
    // i, j+1
    dc.u(i, j+1) = 2;
    // i, j-1
    dc.v(i, j-1) = -1;
    dc.u(i, j-1) = -2; // -> u(2, 1.5) = -1
    // i+1, j
    dc.u(i+1, j) = 0; // -> u(2.5, 2) = 0
    // i-1, j
    dc.u(i-1, j) = 2; // -> u(1.5, 2) = 1
    dc.v(i-1, j) = 1;
    // i+1, j-1
    dc.v(i+1, j-1) = -1; // -> v(2.5, 1) = -1
    // i-1, j+1
    dc.u(i-1, j+1) = 0; // u(1, 2.5) = 1;

    double DuvDy_expected = 1/meshWidth[1]*
                            (0.5*(dc.v(i,j) + dc.v(i+1,j))*0.5*(dc.u(i,j) + dc.u(i,j+1))
                            - 0.5*(dc.v(i,j-1) + dc.v(i+1,j-1))*0.5*(dc.u(i,j-1)+(dc.u(i,j))))
                            + alpha/meshWidth[1]*
                            (0.5*abs(dc.v(i,j)+dc.v(i+1,j))*0.5*(dc.u(i,j)-dc.u(i,j+1))-
                            0.5*abs(dc.v(i,j-1)+dc.v(i+1,j-1))*0.5*(dc.u(i,j-1)-dc.u(i,j)));


    EXPECT_EQ(dc.computeDuvDy(i, j), DuvDy_expected);
};

TEST(DonorCell, ComputeDu2Dx_VaryingAlpha){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    double const alpha {0.5};
    DonorCell dc(n_cells, meshWidth, alpha);
    int const i {2}, j {2};
    dc.u(i-1,j) = -1;
    dc.u(i,j) = 0;
    dc.u(i+1,j) = 1;
    double D2uDx2_expected = 1/meshWidth[0]*
                            (0.25*(pow(dc.u(i,j)+dc.u(i+1,j),2))
                            - 0.25*(pow(dc.u(i-1,j)+dc.u(i,i),2)))
                            + alpha/meshWidth[0]*
                            (0.25*abs(dc.u(i,j)+dc.u(i+1,j))*(dc.u(i,j)-dc.u(i+1,j))
                            - 0.25*abs(dc.u(i-1,j)+dc.u(i,j))*(dc.u(i-1,j)-dc.u(i,j)));
    EXPECT_EQ(dc.computeD2uDx2(2, 2), D2uDx2_expected);
}


TEST(DonorCell, ComputeDuvDx_VaryingAlpha){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    double const alpha {0.5};
    DonorCell dc(n_cells, meshWidth, alpha);
    int const i {2}, j {2};
    dc.v(i,j) = 0;
    dc.v(i-1,j) = -1;
    dc.v(i+1,j) = 3;

    dc.u(i,j) = 0;
    dc.u(i,j+1) = 6;
    // u(i,j+1/2) = 3
    double u_i_jplus = 3;

    dc.u(i-1,j) = -1;
    dc.u(i-1,j+1) = 3;
    // u(i-1, j+1/2) = 1
    double u_iminus1_jplus = 1;

    double left = 3 * dc.v(i,j); // 0
    double right = 1 * dc.v(i-1,j); // -1
    double dc_DuvDx = (left - right) / (meshWidth[0]);

        // original central difference discretization
    double v_iplus_j = 0.5 * (dc.v(i + 1, j) + dc.v(i, j));
    double v_iminus_j = 0.5 * (dc.v(i - 1, j) + dc.v(i, j));
    double cd_DuvDx = (u_i_jplus * v_iplus_j - u_iminus1_jplus * v_iminus_j) / dc.dx();

    double expected = (1 - alpha) * cd_DuvDx + alpha * dc_DuvDx;
    EXPECT_EQ(dc.computeDuvDx(i, j), expected);
}


TEST(DonorCell, ComputeDv2Dy_varyingAlpha){
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    for (int k = 1; k < 11; k++){
        double const alpha {0.1 * k};
        DonorCell dc(n_cells, meshWidth, alpha);
        int const i {2}, j {2};
        dc.v(i,j+1) = 1;
        dc.v(i,j) = 0;
        dc.v(i,j-1) = 1;

        double v_i_jminus = 0.5;
        double v_i_jplus = 0.5;

        double left = 0.5 * dc.v(i,j); // 0
        double right = 0.5 * dc.v(i,j-1); // 0.5

        double dc_Dv2Dy = (left - right) / (meshWidth[1]);

        // original central difference discretization
        double cd_Dv2Dy = (pow(v_i_jplus, 2) - pow(v_i_jminus, 2)) / (meshWidth[1]);
        
        double expected = (1 - alpha) * cd_Dv2Dy + alpha * dc_Dv2Dy;

        EXPECT_EQ(dc.computeDv2Dy(i, j), expected);
    }

}
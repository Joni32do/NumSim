#include "staggered_grid.h"

StaggeredGrid::StaggeredGrid(std::array<int, 2> nCells, std::array<double, 2> meshWidth):
            nCells_(nCells), 
            meshWidth_(meshWidth),
            u_({nCells[0]+1, nCells[1]+2}, {0., -0.5*meshWidth[1]},                 meshWidth),
            v_({nCells[0]+2, nCells[1]+1}, {-0.5*meshWidth[0], 0.},                 meshWidth),
            p_({nCells[0]+2, nCells[1]+2}, {-0.5*meshWidth[0], -0.5*meshWidth[1]},  meshWidth),
            f_({nCells[0]-1, nCells[1]},   {meshWidth[0], 0.5*meshWidth[1]},        meshWidth),
            g_({nCells[0], nCells[1]-1},   {0.5*meshWidth[0], meshWidth[1]},        meshWidth),
            rhs_({nCells[0], nCells[1]},   {0.5*meshWidth[0], 0.5*meshWidth[1]},    meshWidth)
{
    // // Set u
    // std::array<int,2> size_u = {nCells[0]+1, nCells[1]+2};
    // std::array<double,2> origin_u = {0., -0.5*meshWidth[1]};
    // FieldVariable u_(size_u, origin_u, meshWidth);

    // // Set v
    // std::array<int,2> size_v = {nCells[0]+2, nCells[1]+1};
    // std::array<double,2> origin_v = {-0.5*meshWidth[0], 0.};
    // FieldVariable v_(size_v, origin_v, meshWidth);

    // // Set p
    // std::array<int,2> size_p = {nCells[0]+2, nCells[1]+2};
    // std::array<double,2> origin_p = {-0.5*meshWidth[0], -0.5*meshWidth[1]};
    // FieldVariable p_(size_p, origin_p, meshWidth);  

    // // Set f
    // std::array<int,2> size_f = {nCells[0]-1, nCells[1]};
    // std::array<double,2> origin_f = {meshWidth[0], 0.5*meshWidth[1]};
    // FieldVariable f_(size_f, origin_f, meshWidth);

    // // Set g
    // std::array<int,2> size_g = {nCells[0], nCells[1]-1};
    // std::array<double,2> origin_g = {0.5*meshWidth[0], meshWidth[1]};
    // FieldVariable g_(size_g, origin_g, meshWidth);

    // // Set rhs
    // std::array<int,2> size_rhs = {nCells[0], nCells[1]};
    // std::array<double,2> origin_rhs = {0.5*meshWidth[0], 0.5*meshWidth[1]};
    // FieldVariable rhs_(size_rhs, origin_rhs, meshWidth);
}


#include "staggered_grid.h"

StaggeredGrid::StaggeredGrid(std::array<int, 2> nCells, std::array<double, 2> meshWidth):
            nCells_(nCells), 
            meshWidth_(meshWidth),
            u_({nCells[0]+1, nCells[1]+2}, {0., -0.5*meshWidth[1]},                 meshWidth),
            v_({nCells[0]+2, nCells[1]+1}, {-0.5*meshWidth[0], 0.},                 meshWidth),
            p_({nCells[0]+2, nCells[1]+2}, {-0.5*meshWidth[0], -0.5*meshWidth[1]},  meshWidth),
            f_({nCells[0]+1, nCells[1]},   {meshWidth[0], 0.5*meshWidth[1]},        meshWidth),
            g_({nCells[0], nCells[1]+1},   {0.5*meshWidth[0], meshWidth[1]},        meshWidth),
            rhs_({nCells[0], nCells[1]},   {0.5*meshWidth[0], 0.5*meshWidth[1]},    meshWidth)
{
// Delete when you feel ready

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

const std::array<double, 2> StaggeredGrid::meshWidth() const
{
    return meshWidth_;
}


const std::array<int, 2> StaggeredGrid::nCells() const
{
    return nCells_;
}


const FieldVariable & StaggeredGrid::u() const
{
    return u_;
}


const FieldVariable & StaggeredGrid::v() const
{
    return v_;
}


const FieldVariable & StaggeredGrid::p() const
{
    return p_;
}


double StaggeredGrid::u(int i, int j) const
{
    return u_(i, j);
}


double & StaggeredGrid::u(int i, int j)
{
    return u_(i, j);
}


double StaggeredGrid::v(int i, int j) const
{
    return v_(i, j);
}


double & StaggeredGrid::v(int i, int j)
{
    return v_(i, j);
}


double StaggeredGrid::p(int i, int j) const
{
    return p_(i, j);
}


double & StaggeredGrid::p(int i, int j)
{
    return p_(i, j);
}


double & StaggeredGrid::f(int i, int j)
{
    return f_(i, j);
}


double & StaggeredGrid::g(int i, int j)
{
    return g_(i, j);
}


double & StaggeredGrid::rhs(int i, int j)
{
    return rhs_(i, j);
}


double StaggeredGrid::dx() const
{
    return meshWidth_[0];
}


double StaggeredGrid::dy() const
{
    return meshWidth_[1];
}

// TODO: Array always starts with 0
int StaggeredGrid::uIBegin() const
{
    return 0;
}


int StaggeredGrid::uIEnd() const
{
    return nCells_[0];
}


int StaggeredGrid::uJBegin() const
{
    return 0;
}


int StaggeredGrid::uJEnd() const
{
    return nCells_[1]+1;
}


int StaggeredGrid::vIBegin() const
{
    return 0;
}


int StaggeredGrid::vIEnd() const
{
    return nCells_[0]+1;
}


int StaggeredGrid::vJBegin() const
{
    return 0;
}


int StaggeredGrid::vJEnd() const
{
    return nCells_[1];
}


int StaggeredGrid::pIBegin() const
{
    return 0;
}


int StaggeredGrid::pIEnd() const
{
    return nCells_[0]+1;
}


int StaggeredGrid::pJBegin() const
{
    return 0;
}


int StaggeredGrid::pJEnd() const
{
    return nCells_[1]+1;
}
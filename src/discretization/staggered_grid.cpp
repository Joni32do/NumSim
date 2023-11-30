#include "staggered_grid.h"

// TODO: think about setting e.g. p to ncells+2 is both directions since we never access the last elements if
// we set p to ncells+3
StaggeredGrid::StaggeredGrid(std::array<int, 2> nCells, std::array<double, 2> meshWidth) : nCells_(nCells),
                                                                                           meshWidth_(meshWidth),
                                                                                           u_({nCells[0] + 3, nCells[1] + 3}, {-meshWidth[0], -0.5 * meshWidth[1]}, meshWidth),
                                                                                           v_({nCells[0] + 3, nCells[1] + 3}, {-0.5 * meshWidth[0], -meshWidth[1]}, meshWidth),
                                                                                           p_({nCells[0] + 3, nCells[1] + 3}, {-0.5 * meshWidth[0], -0.5 * meshWidth[1]}, meshWidth),
                                                                                           f_({nCells[0] + 3, nCells[1] + 3}, {-meshWidth[0], -0.5 * meshWidth[1]}, meshWidth),
                                                                                           g_({nCells[0] + 3, nCells[1] + 3}, {-0.5 * meshWidth[0], -meshWidth[1]}, meshWidth),
                                                                                           rhs_({nCells[0] + 3, nCells[1] + 3}, {-0.5 * meshWidth[0], -0.5 * meshWidth[1]}, meshWidth)
{
}

const std::array<double, 2> StaggeredGrid::meshWidth() const
{
    return meshWidth_;
}

const std::array<int, 2> StaggeredGrid::nCells() const
{
    return nCells_;
}

const FieldVariable &StaggeredGrid::u() const
{
    return u_;
}

const FieldVariable &StaggeredGrid::v() const
{
    return v_;
}

const FieldVariable &StaggeredGrid::p() const
{
    return p_;
}

const FieldVariable &StaggeredGrid::rhs() const
{
    return rhs_;
}

double StaggeredGrid::u(int i, int j) const
{
    return u_(i, j);
}

double &StaggeredGrid::u(int i, int j)
{
    return u_(i, j);
}

double StaggeredGrid::v(int i, int j) const
{
    return v_(i, j);
}

double &StaggeredGrid::v(int i, int j)
{
    return v_(i, j);
}

double StaggeredGrid::p(int i, int j) const
{
    return p_(i, j);
}

double &StaggeredGrid::p(int i, int j)
{
    return p_(i, j);
}

double &StaggeredGrid::f(int i, int j)
{
    return f_(i, j);
}

double &StaggeredGrid::g(int i, int j)
{
    return g_(i, j);
}

double &StaggeredGrid::rhs(int i, int j)
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

int StaggeredGrid::uIBegin() const
{
    return 1;
}

int StaggeredGrid::uIEnd() const
{
    return nCells_[0] + 2;
}

int StaggeredGrid::uLeftGhost() const
{
    return 0;
}

int StaggeredGrid::uRightGhost() const
{
    return nCells_[0] + 3;
}

int StaggeredGrid::uJBegin() const
{
    return 0;
}

int StaggeredGrid::uJEnd() const
{
    return nCells_[1] + 2;
}

int StaggeredGrid::fIBegin() const
{
    return 1;
}

int StaggeredGrid::fIEnd() const
{
    return nCells_[0] + 2;
}

int StaggeredGrid::fJBegin() const
{
    return 1;
}

int StaggeredGrid::fJEnd() const
{
    return nCells_[1] + 1;
}

int StaggeredGrid::vIBegin() const
{
    return 0;
}

int StaggeredGrid::vIEnd() const
{
    return nCells_[0] + 2;
}

int StaggeredGrid::vJBegin() const
{
    return 1;
}

int StaggeredGrid::vJEnd() const
{
    return nCells_[1] + 2;
}

int StaggeredGrid::vBottomGhost() const
{
    return 0;
}

int StaggeredGrid::vTopGhost() const
{
    return nCells_[1] + 3;
}

int StaggeredGrid::gIBegin() const
{
    return 1;
}

int StaggeredGrid::gIEnd() const
{
    return nCells_[0] + 1;
}

int StaggeredGrid::gJBegin() const
{
    return 1;
}

int StaggeredGrid::gJEnd() const
{
    return nCells_[1] + 2;
}

int StaggeredGrid::pIBegin() const
{
    return 0;
}

int StaggeredGrid::pIEnd() const
{
    return nCells_[0] + 2;
}

int StaggeredGrid::pJBegin() const
{
    return 0;
}

int StaggeredGrid::pJEnd() const
{
    return nCells_[1] + 2;
}

int StaggeredGrid::rhsIBegin() const
{
    return 1;
}

int StaggeredGrid::rhsIEnd() const
{
    return nCells_[0] + 1;
}

int StaggeredGrid::rhsJBegin() const
{
    return 1;
}

int StaggeredGrid::rhsJEnd() const
{
    return nCells_[1] + 1;
}
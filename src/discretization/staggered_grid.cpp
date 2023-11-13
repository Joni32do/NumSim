#include "staggered_grid.h"
#include <iostream>

StaggeredGrid::StaggeredGrid(std::array<int, 2> nCells, std::array<double, 2> meshWidth):
            nCells_(nCells), 
            meshWidth_(meshWidth),
            u_({nCells[0]+2, nCells[1]+2}, { 0., -0.5*meshWidth[1]},               meshWidth),
            v_({nCells[0]+2, nCells[1]+2}, {-0.5*meshWidth[0], 0.},                meshWidth),
            p_({nCells[0]+2, nCells[1]+2}, {-0.5*meshWidth[0], -0.5*meshWidth[1]}, meshWidth),
            f_({nCells[0]+2, nCells[1]+2}, { 0., -0.5*meshWidth[1]},               meshWidth),
            g_({nCells[0]+2, nCells[1]+2}, {-0.5*meshWidth[0], 0},                 meshWidth),
            rhs_({nCells[0]+2, nCells[1]+2},{-0.5*meshWidth[0], -0.5*meshWidth[1]},meshWidth)
{
    // debug indices
    #ifndef NDEBUG
        std::cout << std::endl << ">>>>> Debug Field Variable Boundaries >>>>>" << std::endl << std::endl;

        // nCells (given in settings file)
        std::cout << "User input for number of cells:" << std::endl;
        std::cout << "\tnCellsX = " << nCells_[0] << std::endl;
        std::cout << "\tnCellsY = " << nCells_[1] << std::endl;
        std::cout << std::endl;


        // limits for u
        std::cout << "Indices for u:" << std::endl;
        std::cout << "\tuIBegin = " << uIBegin() << ", uIEnd = " << uIEnd() << std::endl;
        std::cout << "\tuJBegin = " << uJBegin() << ", uJEnd = " << uJEnd() << std::endl;
        std::cout << std::endl;

        // limits for v
        std::cout << "Indices for v:" << std::endl;
        std::cout << "\tvIBegin = " << vIBegin() << ", vIEnd = " << vIEnd() << std::endl;
        std::cout << "\tvJBegin = " << vJBegin() << ", vJEnd = " << vJEnd() << std::endl;
        std::cout << std::endl;

        // limits for p
        std::cout << "Indices for p:" << std::endl;
        std::cout << "\tpIBegin = " << pIBegin() << ", pIEnd = " << pIEnd() << std::endl;
        std::cout << "\tpJBegin = " << pJBegin() << ", pJEnd = " << pJEnd() << std::endl;
        std::cout << std::endl;

        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl << std::endl;
    #endif
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


const FieldVariable & StaggeredGrid::rhs() const
{
    return rhs_;
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

int StaggeredGrid::uIBegin() const
{
    return 0;
}


int StaggeredGrid::uIEnd() const
{
    return nCells_[0]+1;
}


int StaggeredGrid::uJBegin() const
{
    return 0;
}


int StaggeredGrid::uJEnd() const
{
    return nCells_[1]+2;
}


int StaggeredGrid::fIBegin() const
{
    return 0;
}


int StaggeredGrid::fIEnd() const
{
    return nCells_[0]+1;
}


int StaggeredGrid::fJBegin() const
{
    return 1;
}


int StaggeredGrid::fJEnd() const
{
    return nCells_[1]+1;
}


int StaggeredGrid::vIBegin() const
{
    return 0;
}


int StaggeredGrid::vIEnd() const
{
    return nCells_[0]+2;
}


int StaggeredGrid::vJBegin() const
{
    return 0;
}


int StaggeredGrid::vJEnd() const
{
    return nCells_[1]+1;
}


int StaggeredGrid::gIBegin() const
{
    return 1;
}


int StaggeredGrid::gIEnd() const
{
    return nCells_[0]+1;
}


int StaggeredGrid::gJBegin() const
{
    return 0;
}


int StaggeredGrid::gJEnd() const
{
    return nCells_[1]+1;
}


int StaggeredGrid::pIBegin() const
{
    return 0;
}


int StaggeredGrid::pIEnd() const
{
    return nCells_[0]+2;
}


int StaggeredGrid::pJBegin() const
{
    return 0;
}


int StaggeredGrid::pJEnd() const
{
    return nCells_[1]+2;
}


int StaggeredGrid::rhsIBegin() const
{
    return 1;
}


int StaggeredGrid::rhsIEnd() const
{
    return nCells_[0]+1;
}


int StaggeredGrid::rhsJBegin() const
{
    return 1;
}


int StaggeredGrid::rhsJEnd() const
{
    return nCells_[1]+1;
}
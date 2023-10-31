#include <array>
#include <vector>
#include "field_variable.h"

class StaggeredGrid
{
public:
    StaggeredGrid(std::array<int, 2> nCells, std::array<double, 2> meshWidth);
    // get the mesh width, i.e. the length of a single cell in x and y direction 
    const std::array<double, 2> meshWidth() const;
    // get number of cells in each coordinate direction
    const std::array<int, 2> nCells() const;

    // get the field variables
    const FieldVariable & u() const;
    const FieldVariable & v() const;
    const FieldVariable & p() const;
    const FieldVariable & f() const;
    const FieldVariable & g() const;
    
    double u(int i, int j) const;
    double & u(int i, int j);

    double v(int i, int j) const;
    double & v(int i, int j);

    double p(int i, int j) const;
    double & p(int i, int j);

    double & f(int i, int j);
    double & g(int i, int j);
    double & rhs(int i, int j);

    double dx(int i, int j) const;
    double dy(int i, int j) const;


    // get index

    // -- u --
    // first valid index for u in x direction
    int uIBegin() const;
    // one after last valid index for u in x direction
    int uIEnd() const;

    // first valid index for u in y direction
    int uJBegin() const;
    // one after last valid index for u in y direction
    int uJEnd() const;

    // -- v --
    // first valid index for v in x direction
    int vIBegin() const;
    // one after last valid index for v in x direction
    int vIEnd() const;

    // first valid index for v in y direction
    int vJBegin() const;
    // one after last valid index for v in y direction
    int vJEnd() const;

    // -- p --
    // first valid index for p in x direction
    int pIBegin() const;
    // one after last valid index for p in x direction
    int pIEnd() const;
    
    // first valid index for p in y direction
    int pJBegin() const;
    // one after last valid index for p in y direction
    int pJEnd() const;

private:
    const std::array<int, 2> nCells_;
    const std::array<double, 2> meshWidth_;
    FieldVariable u_;
    FieldVariable v_;
    FieldVariable p_;
    FieldVariable f_;
    FieldVariable g_;
    FieldVariable rhs_;
};

}
#include <array>
#include <vector>
#include <iostream>
#include "../storage/field_variable.h"

/**
 * @class StaggeredGrid
 * @brief Create field variables and define limits for the indices
 *
 * Create the necessary u, v, p, f, g, rhs field variables and
 * define the first valid index for each of the field variables,
 * as well as the (one after) last valid index
 */
class StaggeredGrid
{
public:
    /**
     * @brief Constructor
     *
     * @param nCells array containing number of cells in x and y directions
     * @param meshWidth array containing the length of a single cell in x and y directions
     */
    StaggeredGrid(std::array<int, 2> nCells, std::array<double, 2> meshWidth);

    /**
     * @brief  Get the length of a single cell in x and y directions
     */
    const std::array<double, 2> meshWidth() const;
    /**
     * @brief  Get number of cells in x and y directions
     */
    const std::array<int, 2> nCells() const;

    /**
     * @brief  Get a reference to the field variable u
     */
    const FieldVariable &u() const;
    /**
     * @brief  Get a reference to the field variable v
     */
    const FieldVariable &v() const;
    /**
     * @brief  Get a reference to the field variable p
     */
    const FieldVariable &p() const;
    /**
     * @brief  Get a reference to the field variable rhs
     */
    const FieldVariable &rhs() const;

    /**
     * @brief  Access value of u in element (i,j), declared constant
     */
    double u(int i, int j) const;
    /**
     * @brief  Access value of u in element (i,j)
     */
    double &u(int i, int j);

    /**
     * @brief  Access value of v in element (i,j), declared constant
     */
    double v(int i, int j) const;
    /**
     * @brief  Access value of v in element (i,j)
     */
    double &v(int i, int j);

    /**
     * @brief  Access value of p in element (i,j), declared constant
     */
    double p(int i, int j) const;
    /**
     * @brief  Access value of p in element (i,j)
     */
    double &p(int i, int j);

    /**
     * @brief  Access value of f in element (i,j)
     */
    double &f(int i, int j);
    /**
     * @brief  Access value of g in element (i,j)
     */
    double &g(int i, int j);
    /**
     * @brief  Access value of rhs in element (i,j)
     */
    double &rhs(int i, int j);

    /**
     * @brief  Get the mesh width in x-direction
     */
    double dx() const;
    /**
     * @brief  Get the mesh width in y-direction
     */
    double dy() const;

    /**
     * @brief  first valid index for u in x direction
     */
    int uIBegin() const;
    /**
     * @brief  one after last valid index for u in x direction
     */
    int uIEnd() const;

    /**
     * @brief  first valid index for u in y direction
     */
    int uJBegin() const;
    /**
     * @brief  one after last valid index for u in y direction
     */
    int uJEnd() const;

    /**
     * @brief  first valid index for v in x direction
     */
    int vIBegin() const;
    /**
     * @brief  one after last valid index for v in x direction
     */
    int vIEnd() const;

    /**
     * @brief  first valid index for v in y direction
     */
    int vJBegin() const;
    /**
     * @brief  one after last valid index for v in y direction
     */
    int vJEnd() const;

    /**
     * @brief  first valid index for p in x direction
     */
    int pIBegin() const;
    /**
     * @brief  one after last valid index for p in x direction
     */
    int pIEnd() const;

    /**
     * @brief  first valid index for p in y direction
     */
    int pJBegin() const;
    /**
     * @brief  one after last valid index for p in y direction
     */
    int pJEnd() const;

    /**
     * @brief  first valid index for f in x direction
     */
    int fIBegin() const;
    /**
     * @brief  one after last valid index for f in x direction
     */
    int fIEnd() const;

    /**
     * @brief  first valid index for f in y direction
     */
    int fJBegin() const;
    /**
     * @brief  one after last valid index for f in y direction
     */
    int fJEnd() const;

    /**
     * @brief  first valid index for g in x direction
     */
    int gIBegin() const;
    /**
     * @brief  one after last valid index for g in x direction
     */
    int gIEnd() const;

    /**
     * @brief  first valid index for g in y direction
     */
    int gJBegin() const;
    /**
     * @brief  one after last valid index for g in y direction
     */
    int gJEnd() const;

    /**
     * @brief  first valid index for rhs in x direction
     */
    int rhsIBegin() const;
    /**
     * @brief  one after last valid index for rhs in x direction
     */
    int rhsIEnd() const;

    /**
     * @brief  first valid index for rhs in y direction
     */
    int rhsJBegin() const;
    /**
     * @brief  one after last valid index for rhs in y direction
     */
    int rhsJEnd() const;

protected:
    const std::array<int, 2> nCells_;       //!< array containing number of cells in x and y directions
    const std::array<double, 2> meshWidth_; //!< array containing the sizes of cell edges in x and y directions
    FieldVariable u_;                       //!< instance of the field variable u
    FieldVariable v_;                       //!< instance of the field variable v
    FieldVariable p_;                       //!< instance of the field variable p
    FieldVariable f_;                       //!< instance of the field variable f
    FieldVariable g_;                       //!< instance of the field variable g
    FieldVariable rhs_;                     //!< instance of the field variable rhs
};

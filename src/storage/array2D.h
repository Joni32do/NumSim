#pragma once

#include <vector>
#include <array>
#include <cassert>
#include <iostream>

/**
 * @class Array2D
 * @brief This class represents a 2D array of double values.
 *
 * Internally they are stored consecutively in memory.
 * The entries can be accessed by two indices i,j.
 */
class Array2D
{
public:
    /**
     * @brief constructor.
     *
     * @param size size of array in x and y direction
     */
    Array2D(std::array<int, 2> size);

    /**
     * @brief set array value.
     *        Overloads the () operator
     *
     * @param i index in x direction
     * @param j index in y direction
     */
    double &operator()(int i, int j);

    /**
     * @brief get array value.
     *        Overloads the () operator
     *
     * @param i index in x direction
     * @param j index in y direction
     */
    double operator()(int i, int j) const;

    /**
     * @brief get size of array in x and y direction
     */
    std::array<int, 2> size() const;

    /**
     * @brief returns row of array2D as vector
     *
     * @param i index of row to return
     * @param start first element of row
     * @param end last element of row (exclusiv)
     */
    std::vector<double> getRow(int i, int start, int end) const;

    /**
     * @brief returns column of array2D as vector
     *
     * @param i index of row to return
     * @param start first element of row
     * @param end last element of row (exclusiv)
     */
    std::vector<double> getColumn(int j, int start, int end) const;

    /**
     * @brief returns array2D data for communication
     */
    void *data();

protected:
    const std::array<int, 2> size_; //!< size of array in x and y direction
    std::vector<double> data_;      //!< storage array values, in row-major order
};
#include <vector>
#include <array>

/** This class represents a 2D array of double values.
 *  Internally they are stored consecutively in memory.
 *  The entries can be accessed by two indices i,j.
 */
class Array2D
{
    public:
        Array2D(std::array<int,2> size);
        //! setter
        double &operator()(int i, int j);
        //! getter
        double operator()(int i, int j) const;
        //! size getter
        std::array<int,2> size() const;
    
    protected:
        const std::array<int,2> size_;    //< width, height of the domain
        std::vector<double> data_;  //< storage array values, in row-major order
};
#include <vector>
#include <array>

class Array2D
{
    public:
        Array2D(std::array<int,2> size);
        // setter
        double operator()(int i, int j);
        // getter
        double & operator()(int i, int j) const;
        // size getter
        std::array<int,2> size() const;
    
    private:
        const std::array<int,2> size_;
        std::vector<double> data_;

};
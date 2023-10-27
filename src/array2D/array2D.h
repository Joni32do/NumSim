#include <vector>

class Array2D
{
    public:
        Array2D(const int n_x, const int n_y);
        void set_point(int i, int j, double value);
        double get_point(int i, int j);
    
    private:
        const int n_x, n_y;
        std::vector<double> points;

};
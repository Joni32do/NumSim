#pragma once 

class Discretization
{
    public:
        Discretization();
        auto nCells();
        auto u();
        auto v();
        auto p();
        auto f();   //not pretty but implemented like this in the output_writer
        auto g();   //not pretty but implemented like this in the output_writer
};
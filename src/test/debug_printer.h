#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>


class Printer {
public:
    Printer(int ownRankNo);

    // Function to add a new value and string to print
    void add_new_parameter_to_print(const std::string& str);

    // Function to print all values and strings
    void print_values_and_strings() const;

    void save_values_to_file() const;

private:
    std::vector<std::string> strings;
    const int ownRankNo_;
    std::string fileName;
};
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

/**
 * @class Printer
 * @brief Printer used for debugging
 *
 * Saves output to different files depending on rank
 */
class Printer
{
public:
    /**
     * @brief Constructor
     *
     * @param ownRankNo Rank of the process that is wrting to the printer
     */
    Printer(int ownRankNo);

    /**
     * @brief adds new string to the output vector
     */
    void add_new_parameter_to_print(const std::string &str);

    /**
     * @brief adds new double to the output vector
     */
    void add_new_double_to_print(const double &double_);

    /**
     * @brief prints all strings stored in the strings_ vector
     */
    void print_values_and_strings() const;

    /**
     * @brief saves all values in string to file with ownRankNo as name
     */
    void save_values_to_file() const;

    /**
     * @brief saves all doubles in string to file with ownRankNo as name
     */
    void save_doubles_to_file() const;

private:
    std::vector<std::string> strings; //!< vector of strings
    std::vector<double> doubles_;     //!< vector of doubles
    const int ownRankNo_;             //!< Number od the rank that is using the printer
    std::string fileName;             //!< file name to save values to
};
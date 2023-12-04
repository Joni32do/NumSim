#include "debug_printer.h"


Printer::Printer(int ownRankNo):ownRankNo_(ownRankNo) {
    fileName = "../src/test/debug_out/rank_" + std::to_string(ownRankNo_); 
}


void Printer::add_new_parameter_to_print(const std::string& str) {
    strings.push_back(str);
}

void Printer::print_values_and_strings() const {
    for (size_t i = 0; i < strings.size(); ++i) {
        std::cout << strings[i] << std::endl;
    }
}

void Printer::save_values_to_file() const {
    std::ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        std::cerr << "Error opening the file: " << fileName << std::endl;
        return;
    }

    for (size_t i = 0; i < strings.size(); ++i) {
        outputFile << strings[i] << std::endl;
    }

    // Close the file
    outputFile.close();
}

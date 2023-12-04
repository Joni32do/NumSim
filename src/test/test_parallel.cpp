#include "test_parallel.h"

TestParallel::TestParallel(){

};

void TestParallel::testCommunicationVector(){

    Communicator com = Communicator();
    Printer debug = Printer(com.ownRankNo());

    std::vector<double> test = {1.0 * com.ownRankNo(), 1.0 * com.nRanks()};

    if (com.ownRankNo() == 0){
        com.sendTo(1, test);
    } else if (com.ownRankNo() == 1){
        test = com.receiveFrom(0, 2);
    }

    debug.add_new_parameter_to_print(std::to_string(test[0]));
    debug.save_values_to_file();
}

void TestParallel::testRowAndColumn(){
    Communicator com = Communicator();
    Printer debug = Printer(com.ownRankNo());

    std::array<int, 2> nCells = {10, 10};
    std::array<double, 2> meshWidth = {0.1, 0.1};
    double alpha = 0.5;
    DonorCell dc = DonorCell(nCells, meshWidth, alpha);
    dc.u(0,1) = 5.0;
    dc.u(0,5) = -3.5;
    dc.u(5,0) = 2.1;
    std::vector<double> buffer = dc.u().getColumn(0, 0, 12);
    std::vector<double> vertical = dc.u().getRow(0, 0, 12);


    std::stringstream bufferSS;
    for (auto it = buffer.begin(); it != buffer.end(); it++)    {
        if (it != buffer.begin()) {
            bufferSS << " ";
        }
        bufferSS << *it;
    }
    std::stringstream verticalSS;
    for (auto it = buffer.begin(); it != buffer.end(); it++)    {
        if (it != buffer.begin()) {
            verticalSS << " ";
        }
        verticalSS << *it;
    }

    debug.add_new_parameter_to_print(bufferSS.str());
    debug.add_new_parameter_to_print(verticalSS.str());
    debug.save_values_to_file();

}
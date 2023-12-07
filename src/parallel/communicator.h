#pragma once

#include <mpi.h>

#include <memory>
#include <array>
#include <vector>
#include <iostream>

class Communicator
{

public:
    Communicator();

    /**
     * @brief information about ranm
     * @return (int) rank
     */
    int ownRankNo();

    /**
     * @brief information about all processes
     * @return (int) number of total ranks of the programm
     */
    int nRanks();

    /**
     * @brief global minimum from all processes
     */
    double getGlobalMin(double local_value);

    /**
     * @brief global maximum from all processes
     */
    double getGlobalMax(double local_value);

    /**
     * @brief global sum from all processes
     */
    double getGlobalSum(double local_value);

    /**
     * @brief wrapper for MPI send functions
     *
     * can implement both MPI_Isend and MPI_send
     *
     * @requires waitForAll() if Isend is used
     */
    void sendTo(int rank, std::vector<double> buffer, MPI_Request &request); // char direction='T'

    /**
     * @brief wrapper for MPI receive functions
     *
     * @requires waitForAll() if Irecv is used
     */
    std::vector<double> receiveFrom(int rank, int message_size, MPI_Request &request);

    /**
     * @brief wrapper for MPI send functions
     *
     * can implement both MPI_Isend and MPI_send
     *
     * @requires waitForAll() if Isend is used
     */
    void sendTo(int rank, std::vector<double> buffer); // char direction='T'

    /**
     * @brief wrapper for MPI receive functions
     *
     * @requires waitForAll() if Irecv is used
     */
    std::vector<double> receiveFrom(int rank, int message_size);

    /**
     * @brief wrapper for MPI wait
     */
    void wait(MPI_Request &request);

private:
    int ownRankNo_; //!< rank number of process
    int nRanks_;    //!< total number of ranks
};

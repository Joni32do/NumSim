#include "communicator.h"

Communicator::Communicator()
{
    MPI_Comm_rank(MPI_COMM_WORLD, &ownRankNo_);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks_);
}

int Communicator::ownRankNo()
{
    return ownRankNo_;
}

int Communicator::nRanks()
{
    return nRanks_;
}

double Communicator::getGlobalMin(double local_value)
{
    double global_min;
    MPI_Allreduce(&local_value, &global_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
    return global_min;
}

double Communicator::getGlobalMax(double local_value)
{
    double global_max;
    MPI_Allreduce(&local_value, &global_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    return global_max;
}

double Communicator::getGlobalSum(double local_value)
{
    double global_sum;
    MPI_Allreduce(&local_value, &global_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    return global_sum;
}

std::vector<double> Communicator::receiveFrom(int rank, int message_size, MPI_Request &request)
{

    std::vector<double> received_data(message_size);
    MPI_Irecv(received_data.data(), message_size, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, &request);
    return received_data;
}

std::vector<double> Communicator::receiveFrom(int rank, int message_size)
{

    std::vector<double> received_data(message_size);
    MPI_Recv(received_data.data(), message_size, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return received_data;
}

void Communicator::sendTo(int rank, std::vector<double> buffer, MPI_Request &request)
{
    MPI_Isend(buffer.data(), buffer.size(), MPI_DOUBLE, rank, 0, MPI_COMM_WORLD, &request);
}

void Communicator::sendTo(int rank, std::vector<double> buffer)
{
    MPI_Send(buffer.data(), buffer.size(), MPI_DOUBLE, rank, 0, MPI_COMM_WORLD);
}

void Communicator::wait(MPI_Request &request)
{
    MPI_Wait(&request, MPI_STATUS_IGNORE);
}

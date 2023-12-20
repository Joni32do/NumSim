#include "red_black.h"

RedBlack::RedBlack(const std::shared_ptr<Discretization> &data,
                   double epsilon,
                   int maximumNumberOfIterations,
                   std::shared_ptr<Communicator> communicator,
                   std::shared_ptr<Partitioning> partitioning) : PressureSolver(data, epsilon, maximumNumberOfIterations),
                                                       communicator_(communicator),
                                                       partitioning_(partitioning)
{
}

void RedBlack::solve()
{
    setBoundaryValues();

    int n = 0;
    int currentModulo;
    double d_fac = (dx2 * dy2) / (2 * (dx2 + dy2));
    double omegaOpt = 2 / (1 + std::sin(M_PI * (discretization_->dx() + discretization_->dy()) / 2));
    bool startWithRed = partitioning_->lowerLeftIsRed();

    double res = sqrt(communicator_->getGlobalSum(calculateResiduum()));

    while (n < maximumNumberOfIterations_ && res > epsilon_)
    {
        for (int redBlack = 0; redBlack < 2; redBlack++)
        {
            if (startWithRed)
            {
                currentModulo = redBlack;
            }
            else
            {
                currentModulo = (redBlack + 1) % 2;
            }
            for (int j = j_beg; j < j_end; j++)
            {
                int i_start = i_beg + (currentModulo + j) % 2;
                for (int i = i_start; i < i_end; i += 2)
                {

                    double p_x = 1 / dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                    double p_y = 1 / dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1));

                    discretization_->p(i, j) = (1 - omegaOpt) * discretization_->p(i, j) + omegaOpt * (d_fac * (p_x + p_y - discretization_->rhs(i, j)));
                }
            }
            exchangeGhost();
            setBoundaryValues();
        }
        res = sqrt(communicator_->getGlobalSum(calculateResiduum()));
        n++;
    }

#ifndef NDEBUG
    if (communicator_->ownRankNo() == 0)
    {
        std::cout << "[Solver] Number of iterations: " << n << ", final residuum: " << res << std::endl;
        std::cout << "\t"
                  << "Omega Optimal: " << omegaOpt << std::endl;
    }
#endif
}

void RedBlack::setBoundaryValues()
{

    if (partitioning_->ownPartitionContainsBottomBoundary())
    {
        for (int i = discretization_->pIBegin(); i < discretization_->pIEnd(); i++)
        {
            discretization_->p(i, discretization_->pJBegin()) = discretization_->p(i, discretization_->pJBegin() + 1);
        }
    }

    if (partitioning_->ownPartitionContainsTopBoundary())
    {
        for (int i = discretization_->pIBegin(); i < discretization_->pIEnd(); i++)
        {
            discretization_->p(i, discretization_->pJEnd() - 1) = discretization_->p(i, discretization_->pJEnd() - 2);
        }
    }

    if (partitioning_->ownPartitionContainsLeftBoundary())
    {
        for (int j = discretization_->pJBegin(); j < discretization_->pJEnd(); j++)
        {
            discretization_->p(discretization_->pIBegin(), j) = discretization_->p(discretization_->pIBegin() + 1, j);
        }
    }

    if (partitioning_->ownPartitionContainsRightBoundary())
    {
        for (int j = discretization_->pJBegin(); j < discretization_->pJEnd(); j++)
        {
            discretization_->p(discretization_->pIEnd() - 1, j) = discretization_->p(discretization_->pIEnd() - 2, j);
        }
    }
}

void RedBlack::exchangeGhost()
{
    int row_count = i_end - i_beg;
    int col_count = j_end - j_beg;

    MPI_Request requestBottom;
    MPI_Request requestTop;
    MPI_Request requestLeft;
    MPI_Request requestRight;

    MPI_Request sendRequestBottom;
    MPI_Request sendRequestTop;
    MPI_Request sendRequestLeft;
    MPI_Request sendRequestRight;

    std::vector<double> bufferReceiveBottom(row_count, 0);
    std::vector<double> bufferReceiveTop(row_count, 0);
    std::vector<double> bufferReceiveLeft(col_count, 0);
    std::vector<double> bufferReceiveRight(col_count, 0);

    if (!partitioning_->ownPartitionContainsBottomBoundary())
    {

        std::vector<double> buffer = discretization_->p().getRow(j_beg, i_beg, i_end);

        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(),
                              buffer,
                              sendRequestBottom);

        bufferReceiveBottom = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(),
                                                         row_count,
                                                         requestBottom);
    }

    if (!partitioning_->ownPartitionContainsTopBoundary())
    {

        std::vector<double> buffer = discretization_->p().getRow(j_end - 1, i_beg, i_end);

        communicator_->sendTo(partitioning_->topNeighbourRankNo(),
                              buffer,
                              sendRequestTop);

        bufferReceiveTop = communicator_->receiveFrom(partitioning_->topNeighbourRankNo(),
                                                      row_count,
                                                      requestTop);
    }

    if (!partitioning_->ownPartitionContainsLeftBoundary())
    {

        std::vector<double> buffer = discretization_->p().getColumn(i_beg, j_beg, j_end);

        communicator_->sendTo(partitioning_->leftNeighbourRankNo(),
                              buffer,
                              sendRequestLeft);

        bufferReceiveLeft = communicator_->receiveFrom(partitioning_->leftNeighbourRankNo(),
                                                       col_count,
                                                       requestLeft);
    }

    if (!partitioning_->ownPartitionContainsRightBoundary())
    {

        std::vector<double> buffer = discretization_->p().getColumn(i_end - 1, j_beg, j_end);

        communicator_->sendTo(partitioning_->rightNeighbourRankNo(),
                              buffer,
                              sendRequestRight);

        bufferReceiveRight = communicator_->receiveFrom(partitioning_->rightNeighbourRankNo(),
                                                        col_count,
                                                        requestRight);
    }

    // Write in the buffer
    if (!partitioning_->ownPartitionContainsBottomBoundary())
    {
        communicator_->wait(requestBottom);
        for (int i = i_beg; i < i_end; i++)
        {
            discretization_->p(i, j_beg - 1) = bufferReceiveBottom[i - i_beg];
        }
    }
    if (!partitioning_->ownPartitionContainsTopBoundary())
    {
        communicator_->wait(requestTop);
        for (int i = i_beg; i < i_end; i++)
        {
            discretization_->p(i, j_end) = bufferReceiveTop[i - i_beg];
        }
    }

    if (!partitioning_->ownPartitionContainsLeftBoundary())
    {
        communicator_->wait(requestLeft);
        for (int j = j_beg; j < j_end; j++)
        {
            discretization_->p(i_beg - 1, j) = bufferReceiveLeft[j - j_beg];
        }
    }

    if (!partitioning_->ownPartitionContainsRightBoundary())
    {
        communicator_->wait(requestRight);
        for (int j = j_beg; j < j_end; j++)
        {
            discretization_->p(i_end, j) = bufferReceiveRight[j - j_beg];
        }
    }

    // Wait for send message such that it doesnt kill the buffer
    if (!partitioning_->ownPartitionContainsBottomBoundary())
    {
        communicator_->wait(sendRequestBottom);
    }
    if (!partitioning_->ownPartitionContainsTopBoundary())
    {
        communicator_->wait(sendRequestTop);
    }
    if (!partitioning_->ownPartitionContainsLeftBoundary())
    {
        communicator_->wait(sendRequestLeft);
    }
    if (!partitioning_->ownPartitionContainsRightBoundary())
    {
        communicator_->wait(sendRequestRight);
    }
}

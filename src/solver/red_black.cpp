#include "red_black.h"
// TODO: remove
#include "output_writer/output_writer_text_parallel.h"
#include "debugging/debug_printer.h"

RedBlack::RedBlack(const std::shared_ptr<Discretization> &data,
                         double epsilon,
                         int maximumNumberOfIterations,
                         std::shared_ptr<Communicator> communicator,
                         std::shared_ptr<Partitioning> partitioning,
                         std::shared_ptr<Printer> printer): 
                PressureSolver(data, epsilon, maximumNumberOfIterations),
                communicator_(communicator),
                partitioning_(partitioning),
                printer_(printer)
{
}

void RedBlack::solve()
{
    OutputWriterTextParallel out = OutputWriterTextParallel(discretization_, *partitioning_);
    setBoundaryValues();
        

    int n = 0;
    int currentModulo;
    double d_fac = (dx2 * dy2) / (2 * (dx2 + dy2));
    double omegaOpt = 2/(1 + std::sin(M_PI * (discretization_->dx() + discretization_->dy())/2));
    bool startWithRed = partitioning_->lowerLeftIsRed();

    double res = sqrt(communicator_->getGlobalSum(calculateResiduum()));


    while (n < maximumNumberOfIterations_ && res > epsilon_) //
    {
        for (int redBlack=0; redBlack < 2; redBlack++){
            if (startWithRed){
                currentModulo = redBlack;
            } else {
                currentModulo = (redBlack+1)%2;
            }


            for (int j = j_beg; j < j_end; j++)
            {
                int i_start = i_beg + (currentModulo + j)%2;
                for (int i = i_start; i < i_end; i+=2)
                {

                    // std::cout << "i, j: " << i << j << std::endl;
                    double p_x = 1 / dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                    double p_y = 1 / dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1));

                    discretization_->p(i, j) = (1 - omegaOpt) * discretization_->p(i, j) + omegaOpt * (d_fac * (p_x + p_y - discretization_->rhs(i, j)));
                    
                }
            }
            exchangeGhost(n);
            setBoundaryValues();
        }

        // Compute the residual with new values
        res = sqrt(communicator_->getGlobalSum(calculateResiduum()));
        n++;
    }


#ifndef NDEBUG
    if (communicator_->ownRankNo() == 0){
        std::cout << "[Solver] Number of iterations: " << n << ", final residuum: " << res << std::endl;
        std::cout << "\t" << "Omega Optimal: " << omegaOpt << std::endl;
    }
#endif
}


void RedBlack::setBoundaryValues(){


    if(partitioning_->ownPartitionContainsBottomBoundary()){
        for(int i = discretization_->pIBegin(); i < discretization_->pIEnd(); i++){
            discretization_->p(i,discretization_->pJBegin()) = discretization_->p(i,discretization_->pJBegin()+1);
        }
    }

    if(partitioning_->ownPartitionContainsTopBoundary()){
        for(int i = discretization_->pIBegin(); i < discretization_->pIEnd(); i++){
            discretization_->p(i,discretization_->pJEnd()-1) = discretization_->p(i,discretization_->pJEnd()-2);
        }
    }

    if(partitioning_->ownPartitionContainsLeftBoundary()){
        for(int j = discretization_->pJBegin(); j<discretization_->pJEnd(); j++){
            discretization_->p(discretization_->pIBegin(), j) = discretization_->p(discretization_->pIBegin()+1, j);
        }
    }

    if(partitioning_->ownPartitionContainsRightBoundary()){
        for(int j = discretization_->pJBegin(); j<discretization_->pJEnd(); j++){
            discretization_->p(discretization_->pIEnd()-1, j) = discretization_->p(discretization_->pIEnd()-2, j);
        }
    }

}

void RedBlack::exchangeGhost(int current_it){
    // TODO: Optimierung -> sende je nach Schritt nur die Hälfte der Daten

    int ref_iteration = 20;
    
    double starttime_bot = MPI_Wtime();
    
    if (!partitioning_->ownPartitionContainsBottomBoundary()){

        std::vector<double> buffer = discretization_->p().getRow(j_beg, i_beg, i_end);
        std::vector<double> buffer_receive;
        int buffer_size = i_end - i_beg;

        double send_time, receive_time;

        if (partitioning_->sendsFirstUpDown()){
            double starttime_send = MPI_Wtime();
            communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer);
            double starttime_receive = MPI_Wtime();
            buffer_receive = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(), buffer_size);
            double receive_end = MPI_Wtime();
            receive_time = receive_end -starttime_receive;
            send_time = starttime_receive-starttime_send;
        } else{
            double starttime_receive = MPI_Wtime();
            buffer_receive = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(), buffer_size);
            double starttime_send = MPI_Wtime();
            communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer);
            double send_time_end = MPI_Wtime();
            send_time = send_time_end-starttime_send;
            send_time = starttime_send-starttime_receive;
        }
        
        // Write the buffer
        for (int i = i_beg; i < i_end; i++){
            discretization_->p(i, j_beg-1) = buffer_receive[i - i_beg];
        }

        if(current_it == ref_iteration){
            double bottom_exchange = MPI_Wtime();
            std::ostringstream bot_time, receive_time_, send_time_;
            bot_time << std::fixed << std::setprecision(10) << bottom_exchange-starttime_bot;
            receive_time_ << std::fixed << std::setprecision(10) << receive_time;
            send_time_ << std::fixed << std::setprecision(10) << send_time;
            printer_->add_new_parameter_to_print("bot exchange: \t" + bot_time.str());
            printer_->add_new_parameter_to_print("bot receive : \t" + receive_time_.str());
            printer_->add_new_parameter_to_print("bot send : \t" + send_time_.str());
        } 
    }
    
    double starttime_top = MPI_Wtime();

    
    if (!partitioning_->ownPartitionContainsTopBoundary()){

        std::vector<double> buffer = discretization_->p().getRow(j_end - 1, i_beg, i_end);
        std::vector<double> buffer_receive;
        int buffer_size = i_end - i_beg;

        if (partitioning_->sendsFirstUpDown()){
            communicator_->sendTo(partitioning_->topNeighbourRankNo(), buffer);
            buffer_receive = communicator_->receiveFrom(partitioning_->topNeighbourRankNo(), buffer_size);
        } else {
            buffer_receive = communicator_->receiveFrom(partitioning_->topNeighbourRankNo(), buffer_size);
            communicator_->sendTo(partitioning_->topNeighbourRankNo(), buffer);
        }

        // Write the buffer
        for (int i = i_beg; i < i_end; i++){
            discretization_->p(i, j_end) = buffer_receive[i - i_beg];
        }

        if(current_it == ref_iteration){
            double top_exchange = MPI_Wtime();
            std::ostringstream top_time;
            top_time << std::fixed << std::setprecision(10) << top_exchange-starttime_top;
            printer_->add_new_parameter_to_print("top exchange: \t" + top_time.str());
        }
    }
    
    double starttime_left = MPI_Wtime();

    
    if (!partitioning_->ownPartitionContainsLeftBoundary()){

        std::vector<double> buffer = discretization_->p().getColumn(i_beg, j_beg, j_end);
        std::vector<double> buffer_receive;
        int buffer_size = j_end - j_beg;

        if (partitioning_->sendsFirstLeftRight()){
            communicator_->sendTo(partitioning_->leftNeighbourRankNo(), buffer);
            buffer_receive = communicator_->receiveFrom(partitioning_->leftNeighbourRankNo(), buffer_size);
        } else {
            buffer_receive = communicator_->receiveFrom(partitioning_->leftNeighbourRankNo(), buffer_size);
            communicator_->sendTo(partitioning_->leftNeighbourRankNo(), buffer);
        }
        
        // Write the buffer
        for (int j = j_beg; j < j_end; j++){
            discretization_->p(i_beg - 1, j) = buffer_receive[j - j_beg];
        }

        if(current_it == ref_iteration){
            double left_exchange = MPI_Wtime();
            std::ostringstream left_time;
            left_time << std::fixed << std::setprecision(10) << left_exchange-starttime_left;
            printer_->add_new_parameter_to_print("lef exchange: \t" + left_time.str());
        }
    }
    
    
    double starttime_right = MPI_Wtime();
    
    if (!partitioning_->ownPartitionContainsRightBoundary()){

        std::vector<double> buffer = discretization_->p().getColumn(i_end - 1, j_beg, j_end);
        std::vector<double> buffer_receive;
        int buffer_size = j_end - j_beg;

        if (partitioning_->sendsFirstLeftRight()){
            communicator_->sendTo(partitioning_->rightNeighbourRankNo(), buffer);
            buffer_receive = communicator_->receiveFrom(partitioning_->rightNeighbourRankNo(), buffer_size);
        } else {
            buffer_receive = communicator_->receiveFrom(partitioning_->rightNeighbourRankNo(), buffer_size);
            communicator_->sendTo(partitioning_->rightNeighbourRankNo(), buffer);
        }
        
        // Write the buffer
        for (int j = j_beg; j < j_end; j++){
            discretization_->p(i_end, j) = buffer_receive[j - j_beg];
        }

        if(current_it == ref_iteration){
            double right_exchange = MPI_Wtime();
            std::ostringstream right_time;
            right_time << std::fixed << std::setprecision(10) << right_exchange-starttime_right;
            printer_->add_new_parameter_to_print("rig exchange: \t" + right_time.str());
        }
    }
    
    

    // if(current_it == 20){
    //     std::ostringstream bot_time;
    //     bot_time << std::fixed << std::setprecision(10) << bottom_exchange-starttime;

    //     std::ostringstream top_time;
    //     top_time << std::fixed << std::setprecision(10) << top_exchange-bottom_exchange;

    //     std::ostringstream left_time;
    //     left_time << std::fixed << std::setprecision(10) << left_exchange-top_exchange;

    //     std::ostringstream right_time;
    //     right_time << std::fixed << std::setprecision(10) << right_exchange-left_exchange;


    //     printer_->add_new_parameter_to_print("bot exchange: \t" + bot_time.str());
    //     printer_->add_new_parameter_to_print("top exchange: \t" + top_time.str());
    //     printer_->add_new_parameter_to_print("lef exchange: \t" + left_time.str());
    //     printer_->add_new_parameter_to_print("rig exchange: \t" + right_time.str());
    // }
    
}

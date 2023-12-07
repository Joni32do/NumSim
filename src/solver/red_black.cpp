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
                printer_(printer),
                bufferReceiveBottom(row_count_, 0),
                bufferReceiveTop(row_count_, 0),
                bufferReceiveLeft(col_count_, 0),
                bufferReceiveRight(col_count_, 0),
                bufferBottom(row_count_, 0),
                bufferTop(row_count_, 0),
                bufferLeft(col_count_, 0),
                bufferRight(col_count_, 0)
{

}

void RedBlack::solve()
{
    setBoundaryValues();
        

    int n = 0;
    int startShift;
    double d_fac = (dx2 * dy2) / (2 * (dx2 + dy2));
    double omegaOpt = 2/(1 + std::sin(M_PI * (discretization_->dx() + discretization_->dy())/2));
    bool startWithRed = partitioning_->lowerLeftIsRed();

    double res = sqrt(communicator_->getGlobalSum(calculateResiduum()));

    //TODO: remove
    OutputWriterTextParallel out(discretization_, *partitioning_);
    
    // maximumNumberOfIterations_ = 1;
    
    while (n < maximumNumberOfIterations_ && res > epsilon_)
    {
        double startTimeWhole = MPI_Wtime();


        for (int redBlack=0; redBlack < 2; redBlack++){

            if (startWithRed){
                startShift = redBlack;
            } else {
                startShift = (redBlack+1)%2;
            }
            



            // TODO: remove
            double startTime = MPI_Wtime();
            
            
            
            
            for (int j = j_beg; j < j_end; j++)
            {
                int i_start = i_beg + (startShift + j - j_beg)%2;
                for (int i = i_start; i < i_end; i+=2)
                {

                    double p_x = 1 / dx2 * (discretization_->p(i + 1, j) + discretization_->p(i - 1, j));
                    double p_y = 1 / dy2 * (discretization_->p(i, j + 1) + discretization_->p(i, j - 1));

                    discretization_->p(i, j) = (1 - omegaOpt) * discretization_->p(i, j) + omegaOpt * (d_fac * (p_x + p_y - discretization_->rhs(i, j)));
                    
                }
            }
            double loopTime = MPI_Wtime();
            exchangeGhost();
            double ghostTime = MPI_Wtime();
            setBoundaryValues();
            
            // out.writePressureFile();

        // #ifndef NDEBUG
        //     std::cout << communicator_->ownRankNo() << "Time for loop:  " << loopTime-startTime << std::endl;
        //     std::cout << communicator_->ownRankNo() << "Time for Ghost: " << ghostTime-loopTime << std::endl;
        // #endif
        }

        // Compute the residual with new values
        double beforResidual = MPI_Wtime();
        res = sqrt(communicator_->getGlobalSum(calculateResiduum()));
        double residualTime = MPI_Wtime();
    // #ifndef NDEBUG
    //     std::cout << communicator_->ownRankNo() << "Time for Resid: " << residualTime-beforResidual << std::endl;
    //     std::cout << communicator_->ownRankNo() << "Time for whole: " << residualTime-startTimeWhole << std::endl;
    // #endif
        n++;
    }


#ifndef NDEBUG
    if (communicator_->ownRankNo() == 0){
        std::cout << "[Solver] Number of iterations: " << n << ", final residuum: " << res << std::endl;
        // std::cout << "\t" << "Omega Optimal: " << omegaOpt << std::endl;
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

void RedBlack::exchangeGhost(){

    MPI_Request requestBottom;
    MPI_Request requestTop;
    MPI_Request requestLeft;
    MPI_Request requestRight;

    MPI_Request sendRequestBottom;
    MPI_Request sendRequestTop;
    MPI_Request sendRequestLeft;
    MPI_Request sendRequestRight;


    
    if (!partitioning_->ownPartitionContainsBottomBoundary()){
        // int startBottom = startShift;
        // for (int i = startBottom; i < i_end; i+=2){
        //     bufferBottom[i/2] = discretization_->p(i, j_beg);
        // }


        bufferBottom = discretization_->p().getRow(j_beg, i_beg, i_end);
        // std::cout <<  communicator_->ownRankNo() << " " << startShift << "buffer row bottom   : " << MPI_Wtime()-time_start << std::endl;

        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(),
                              bufferBottom,
                              sendRequestBottom);

        bufferReceiveBottom = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(),
                                                         row_count_,
                                                         requestBottom);
    } 
    
    
    if (!partitioning_->ownPartitionContainsTopBoundary()){
        // int startTop = (startShift+col_count_+1)%2;
        // for (int i = startTop; i < i_end; i+=2){
        //     bufferTop[i/2] = discretization_->p(i, j_end);
        // }
        
        
        // double time_start = MPI_Wtime();
        bufferTop = discretization_->p().getRow(j_end - 1, i_beg, i_end);
        // std::cout <<  communicator_->ownRankNo() << " " <<  startShift << "buffer row    top   : " << MPI_Wtime()-time_start << std::endl;

        communicator_->sendTo(partitioning_->topNeighbourRankNo(),
                              bufferTop,
                              sendRequestTop);

        bufferReceiveTop = communicator_->receiveFrom(partitioning_->topNeighbourRankNo(),
                                                      row_count_,
                                                      requestTop);
    }
    
    if (!partitioning_->ownPartitionContainsLeftBoundary()){
        // int startLeft = startShift;
        // for (int j = startLeft; j < j_end; j+=2){
        //     bufferLeft[j/2] = discretization_->p(i_beg, j);
        // }



        // double time_start = MPI_Wtime();
        bufferLeft = discretization_->p().getColumn(i_beg, j_beg, j_end);
        // std::cout <<  communicator_->ownRankNo() << " " <<  startShift << "buffer column left  : " << MPI_Wtime()-time_start << std::endl;
        
        communicator_->sendTo(partitioning_->leftNeighbourRankNo(),
                              bufferLeft,
                              sendRequestLeft);

        bufferReceiveLeft = communicator_->receiveFrom(partitioning_->leftNeighbourRankNo(),
                                                       col_count_,
                                                       requestLeft);
    }
    
    if (!partitioning_->ownPartitionContainsRightBoundary()){
        // int startRight = (startShift+row_count_+1)%2;
        // for (int j = startRight; j < j_end; j+=2){
        //     bufferRight[j/2] = discretization_->p(i_end - 1, j);
        // }


        // double time_start = MPI_Wtime();
        bufferRight = discretization_->p().getColumn(i_end - 1, j_beg, j_end);
        // std::cout << communicator_->ownRankNo() << " " <<  startShift << "buffer column right : " << MPI_Wtime()-time_start << std::endl;

        communicator_->sendTo(partitioning_->rightNeighbourRankNo(),
                              bufferRight,
                              sendRequestRight);
        
        bufferReceiveRight = communicator_->receiveFrom(partitioning_->rightNeighbourRankNo(),
                                                        col_count_,
                                                        requestRight);
        
    }



    // Write in the buffer
    if (!partitioning_->ownPartitionContainsBottomBoundary()){
        communicator_->wait(requestBottom);
        
        for (int i = i_beg; i < i_end; i++){
            discretization_->p(i, j_beg - 1) = bufferReceiveBottom[i-i_beg];
        }
    }
    if (!partitioning_->ownPartitionContainsTopBoundary()){
        communicator_->wait(requestTop);

        for (int i = i_beg; i < i_end; i++){
            discretization_->p(i, j_end) = bufferReceiveTop[i-i_beg];
        }
    }

    if (!partitioning_->ownPartitionContainsLeftBoundary()){
        communicator_->wait(requestLeft);
        
        for (int j = j_beg; j < j_end; j++){
            discretization_->p(i_beg - 1, j) = bufferReceiveLeft[j-j_beg];
        }
    }

    if (!partitioning_->ownPartitionContainsRightBoundary()){
        communicator_->wait(requestRight);
        
        for (int j = j_beg; j < j_end; j++){
            discretization_->p(i_end, j) = bufferReceiveRight[j-j_beg];
        }
    }



    // Wait for send message such that it doesnt kill the buffer
    if (!partitioning_->ownPartitionContainsBottomBoundary()){
        communicator_->wait(sendRequestBottom);
    }
    if (!partitioning_->ownPartitionContainsTopBoundary()){
        communicator_->wait(sendRequestTop);
    }
    if (!partitioning_->ownPartitionContainsLeftBoundary()){
        communicator_->wait(sendRequestLeft);
    }
    if (!partitioning_->ownPartitionContainsRightBoundary()){
        communicator_->wait(sendRequestRight);
    }
     


    
    
    

    // print

    // std::ostringstream bot_time;
    // bot_time << std::fixed << std::setprecision(10) << bottom_exchange-starttime;

    // std::ostringstream top_time;
    // top_time << std::fixed << std::setprecision(10) << top_exchange-bottom_exchange;

    // std::ostringstream left_time;
    // left_time << std::fixed << std::setprecision(10) << left_exchange-top_exchange;

    // std::ostringstream right_time;
    // right_time << std::fixed << std::setprecision(10) << right_exchange-left_exchange;


    // printer_->add_new_parameter_to_print("bot exchange: \t" + bot_time.str());
    // printer_->add_new_parameter_to_print("top exchange: \t" + top_time.str());
    // printer_->add_new_parameter_to_print("lef exchange: \t" + left_time.str());
    // printer_->add_new_parameter_to_print("rig exchange: \t" + right_time.str());
    
}





// This should work, but unfortunately some values are still wrong, so it diverges


void RedBlack::exchangeGhost(int startShift){

    MPI_Request requestBottom;
    MPI_Request requestTop;
    MPI_Request requestLeft;
    MPI_Request requestRight;

    MPI_Request sendRequestBottom;
    MPI_Request sendRequestTop;
    MPI_Request sendRequestLeft;
    MPI_Request sendRequestRight;

    startShift = (startShift+1)%2;

    
    if (!partitioning_->ownPartitionContainsBottomBoundary()){
        int startBottom = startShift;
        for (int i = startBottom; i < i_end; i+=2){
            bufferBottom[i/2] = discretization_->p(i, j_beg);
        }


        // TODO: setRow mit Referenz
        // bufferBottom = discretization_->p().getRow(j_beg, i_beg, i_end);
        // std::cout <<  communicator_->ownRankNo() << " " << startShift << "buffer row bottom   : " << MPI_Wtime()-time_start << std::endl;

        communicator_->sendTo(partitioning_->bottomNeighbourRankNo(),
                              bufferBottom,
                              sendRequestBottom);

        bufferReceiveBottom = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(),
                                                         row_count_,
                                                         requestBottom);
    } 
    
    
    if (!partitioning_->ownPartitionContainsTopBoundary()){
        int startTop = (startShift+col_count_+1)%2;
        for (int i = startTop; i < i_end; i+=2){
            bufferTop[i/2] = discretization_->p(i, j_end);
        }
        
        
        // double time_start = MPI_Wtime();
        // bufferTop = discretization_->p().getRow(j_end - 1, i_beg, i_end);
        // std::cout <<  communicator_->ownRankNo() << " " <<  startShift << "buffer row    top   : " << MPI_Wtime()-time_start << std::endl;

        communicator_->sendTo(partitioning_->topNeighbourRankNo(),
                              bufferTop,
                              sendRequestTop);

        bufferReceiveTop = communicator_->receiveFrom(partitioning_->topNeighbourRankNo(),
                                                      row_count_,
                                                      requestTop);
    }
    
    if (!partitioning_->ownPartitionContainsLeftBoundary()){
        int startLeft = startShift;
        for (int j = startLeft; j < j_end; j+=2){
            bufferLeft[j/2] = discretization_->p(i_beg, j);
        }



        // double time_start = MPI_Wtime();
        // bufferLeft = discretization_->p().getColumn(i_beg, j_beg, j_end);
        // std::cout <<  communicator_->ownRankNo() << " " <<  startShift << "buffer column left  : " << MPI_Wtime()-time_start << std::endl;
        
        communicator_->sendTo(partitioning_->leftNeighbourRankNo(),
                              bufferLeft,
                              sendRequestLeft);

        bufferReceiveLeft = communicator_->receiveFrom(partitioning_->leftNeighbourRankNo(),
                                                       col_count_,
                                                       requestLeft);
    }
    
    if (!partitioning_->ownPartitionContainsRightBoundary()){
        int startRight = (startShift+row_count_+1)%2;
        for (int j = startRight; j < j_end; j+=2){
            bufferRight[j/2] = discretization_->p(i_end - 1, j);
        }


        // double time_start = MPI_Wtime();
        // bufferRight = discretization_->p().getColumn(i_end - 1, j_beg, j_end);
        // std::cout << communicator_->ownRankNo() << " " <<  startShift << "buffer column right : " << MPI_Wtime()-time_start << std::endl;

        communicator_->sendTo(partitioning_->rightNeighbourRankNo(),
                              bufferRight,
                              sendRequestRight);
        
        bufferReceiveRight = communicator_->receiveFrom(partitioning_->rightNeighbourRankNo(),
                                                        col_count_,
                                                        requestRight);
        
    }



    // Write in the buffer
    if (!partitioning_->ownPartitionContainsBottomBoundary()){
        communicator_->wait(requestBottom);
        
        int startBottom = (startShift+1)%2;
        for (int i = startBottom; i < i_end; i+=2){
            discretization_->p(i, j_beg - 1) = bufferReceiveBottom[i/2];
        }
    }
    if (!partitioning_->ownPartitionContainsTopBoundary()){
        communicator_->wait(requestTop);

        int startTop = (startShift+col_count_)%2;
        for (int i = startTop; i < i_end; i+=2){
            discretization_->p(i, j_end) = bufferReceiveTop[i/2];
        }
    }

    if (!partitioning_->ownPartitionContainsLeftBoundary()){
        communicator_->wait(requestLeft);
        
        int startLeft = (startShift+1)%2;
        for (int j = startLeft; j < j_end; j+=2){
            discretization_->p(i_beg - 1, j) = bufferReceiveLeft[j/2];
        }
    }

    if (!partitioning_->ownPartitionContainsRightBoundary()){
        communicator_->wait(requestRight);
        
        int startRight = (startShift+row_count_)%2;
        for (int j = startRight; j < j_end; j+=2){
            discretization_->p(i_end, j) = bufferReceiveRight[j/2];
        }
    }



    // Wait for send message such that it doesnt kill the buffer
    if (!partitioning_->ownPartitionContainsBottomBoundary()){
        communicator_->wait(sendRequestBottom);
    }
    if (!partitioning_->ownPartitionContainsTopBoundary()){
        communicator_->wait(sendRequestTop);
    }
    if (!partitioning_->ownPartitionContainsLeftBoundary()){
        communicator_->wait(sendRequestLeft);
    }
    if (!partitioning_->ownPartitionContainsRightBoundary()){
        communicator_->wait(sendRequestRight);
    }
     


    
    
    

    // print

    // std::ostringstream bot_time;
    // bot_time << std::fixed << std::setprecision(10) << bottom_exchange-starttime;

    // std::ostringstream top_time;
    // top_time << std::fixed << std::setprecision(10) << top_exchange-bottom_exchange;

    // std::ostringstream left_time;
    // left_time << std::fixed << std::setprecision(10) << left_exchange-top_exchange;

    // std::ostringstream right_time;
    // right_time << std::fixed << std::setprecision(10) << right_exchange-left_exchange;


    // printer_->add_new_parameter_to_print("bot exchange: \t" + bot_time.str());
    // printer_->add_new_parameter_to_print("top exchange: \t" + top_time.str());
    // printer_->add_new_parameter_to_print("lef exchange: \t" + left_time.str());
    // printer_->add_new_parameter_to_print("rig exchange: \t" + right_time.str());
    
}



//////////////////////////////////////////////////////////////////////////////////////////


// GIT


// void RedBlack::exchangeGhost(int current_it){
    
//     int ref_iteration = 20;
    
//     double starttime_bot = MPI_Wtime();
    
//     if (!partitioning_->ownPartitionContainsBottomBoundary()){

//         std::vector<double> buffer = discretization_->p().getRow(j_beg, i_beg, i_end);
//         std::vector<double> buffer_receive;
//         int buffer_size = i_end - i_beg;

//         double send_time, receive_time;

//         if (partitioning_->sendsFirstUpDown()){
//             double starttime_send = MPI_Wtime();


//             communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer);


//             double starttime_receive = MPI_Wtime();
//             buffer_receive = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(), buffer_size);
//             double receive_end = MPI_Wtime();


//             receive_time = receive_end -starttime_receive;
//             send_time = starttime_receive-starttime_send;
//         } else{
//             double starttime_receive = MPI_Wtime();


//             buffer_receive = communicator_->receiveFrom(partitioning_->bottomNeighbourRankNo(), buffer_size);
            
            
//             double starttime_send = MPI_Wtime();
//             communicator_->sendTo(partitioning_->bottomNeighbourRankNo(), buffer);
//             double send_time_end = MPI_Wtime();
            
            
//             send_time = send_time_end-starttime_send;
//             send_time = starttime_send-starttime_receive;
//         }
        
//         // Write the buffer
//         for (int i = i_beg; i < i_end; i++){
//             discretization_->p(i, j_beg-1) = buffer_receive[i - i_beg];
//         }

//         if(current_it == ref_iteration){
//             double bottom_exchange = MPI_Wtime();
//             std::ostringstream bot_time, receive_time_, send_time_;
//             bot_time << std::fixed << std::setprecision(10) << bottom_exchange-starttime_bot;
//             receive_time_ << std::fixed << std::setprecision(10) << receive_time;
//             send_time_ << std::fixed << std::setprecision(10) << send_time;
//             printer_->add_new_parameter_to_print("bot exchange: \t" + bot_time.str());
//             printer_->add_new_parameter_to_print("bot receive : \t" + receive_time_.str());
//             printer_->add_new_parameter_to_print("bot send : \t" + send_time_.str());
//         } 
//     }
    
//     double starttime_top = MPI_Wtime();

    
//     if (!partitioning_->ownPartitionContainsTopBoundary()){

//         std::vector<double> buffer = discretization_->p().getRow(j_end - 1, i_beg, i_end);
//         std::vector<double> buffer_receive;
//         int buffer_size = i_end - i_beg;

//         if (partitioning_->sendsFirstUpDown()){
//             communicator_->sendTo(partitioning_->topNeighbourRankNo(), buffer);
//             buffer_receive = communicator_->receiveFrom(partitioning_->topNeighbourRankNo(), buffer_size);
//         } else {
//             buffer_receive = communicator_->receiveFrom(partitioning_->topNeighbourRankNo(), buffer_size);
//             communicator_->sendTo(partitioning_->topNeighbourRankNo(), buffer);
//         }

//         // Write the buffer
//         for (int i = i_beg; i < i_end; i++){
//             discretization_->p(i, j_end) = buffer_receive[i - i_beg];
//         }

//         if(current_it == ref_iteration){
//             double top_exchange = MPI_Wtime();
//             std::ostringstream top_time;
//             top_time << std::fixed << std::setprecision(10) << top_exchange-starttime_top;
//             printer_->add_new_parameter_to_print("top exchange: \t" + top_time.str());
//         }
//     }
    
//     double starttime_left = MPI_Wtime();

    
//     if (!partitioning_->ownPartitionContainsLeftBoundary()){

//         std::vector<double> buffer = discretization_->p().getColumn(i_beg, j_beg, j_end);
//         std::vector<double> buffer_receive;
//         int buffer_size = j_end - j_beg;

//         if (partitioning_->sendsFirstLeftRight()){
//             communicator_->sendTo(partitioning_->leftNeighbourRankNo(), buffer);
//             buffer_receive = communicator_->receiveFrom(partitioning_->leftNeighbourRankNo(), buffer_size);
//         } else {
//             buffer_receive = communicator_->receiveFrom(partitioning_->leftNeighbourRankNo(), buffer_size);
//             communicator_->sendTo(partitioning_->leftNeighbourRankNo(), buffer);
//         }
        
//         // Write the buffer
//         for (int j = j_beg; j < j_end; j++){
//             discretization_->p(i_beg - 1, j) = buffer_receive[j - j_beg];
//         }

//         if(current_it == ref_iteration){
//             double left_exchange = MPI_Wtime();
//             std::ostringstream left_time;
//             left_time << std::fixed << std::setprecision(10) << left_exchange-starttime_left;
//             printer_->add_new_parameter_to_print("lef exchange: \t" + left_time.str());
//         }
//     }
    
    
//     double starttime_right = MPI_Wtime();
    
//     if (!partitioning_->ownPartitionContainsRightBoundary()){

//         std::vector<double> buffer = discretization_->p().getColumn(i_end - 1, j_beg, j_end);
//         std::vector<double> buffer_receive;
//         int buffer_size = j_end - j_beg;

//         if (partitioning_->sendsFirstLeftRight()){
//             communicator_->sendTo(partitioning_->rightNeighbourRankNo(), buffer);
//             buffer_receive = communicator_->receiveFrom(partitioning_->rightNeighbourRankNo(), buffer_size);
//         } else {
//             buffer_receive = communicator_->receiveFrom(partitioning_->rightNeighbourRankNo(), buffer_size);
//             communicator_->sendTo(partitioning_->rightNeighbourRankNo(), buffer);
//         }
        
//         // Write the buffer
//         for (int j = j_beg; j < j_end; j++){
//             discretization_->p(i_end, j) = buffer_receive[j - j_beg];
//         }

//         if(current_it == ref_iteration){
//             double right_exchange = MPI_Wtime();
//             std::ostringstream right_time;
//             right_time << std::fixed << std::setprecision(10) << right_exchange-starttime_right;
//             printer_->add_new_parameter_to_print("rig exchange: \t" + right_time.str());
//         }
//     }
    
    

//     // if(current_it == 20){
//     //     std::ostringstream bot_time;
//     //     bot_time << std::fixed << std::setprecision(10) << bottom_exchange-starttime;

//     //     std::ostringstream top_time;
//     //     top_time << std::fixed << std::setprecision(10) << top_exchange-bottom_exchange;

//     //     std::ostringstream left_time;
//     //     left_time << std::fixed << std::setprecision(10) << left_exchange-top_exchange;

//     //     std::ostringstream right_time;
//     //     right_time << std::fixed << std::setprecision(10) << right_exchange-left_exchange;


//     //     printer_->add_new_parameter_to_print("bot exchange: \t" + bot_time.str());
//     //     printer_->add_new_parameter_to_print("top exchange: \t" + top_time.str());
//     //     printer_->add_new_parameter_to_print("lef exchange: \t" + left_time.str());
//     //     printer_->add_new_parameter_to_print("rig exchange: \t" + right_time.str());
//     // }
    
// }

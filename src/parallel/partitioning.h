#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <cassert>

#include "communicator.h"
#include "../debugging/debug_printer.h"

class Partitioning
{
public:
  Partitioning(std::array<int,2> nCellsGlobal,
               std::shared_ptr<Communicator> communicator);
  // Use Constructor instead of initialize
  //! compute partitioning, set internal variables
  // void initialize();

  //! get the local number of cells in the own subdomain
  std::array<int,2> nCellsLocal() const;

  //! get the global number of cells in the whole computational domain
  //! used in OutputWriterParaviewParallel
  std::array<int,2> nCellsGlobal() const;

  //! get the own MPI rank no
  //! used in OutputWriterParaviewParallel and OutputWriterTextParallel
  int ownRankNo() const;

  //! number of MPI ranks
  int nRanks() const;

  //! if the own partition has part of the bottom boundary of the whole domain
  bool ownPartitionContainsBottomBoundary() const;

  //! if the own partition has part of the top boundary of the whole domain
  //! used in OutputWriterParaviewParallel
  bool ownPartitionContainsTopBoundary() const;

  //! if the own partition has part of the left boundary of the whole domain
  bool ownPartitionContainsLeftBoundary() const;

  //! if the own partition has part of the right boundary of the whole domain
  //! used in OutputWriterParaviewParallel
  bool ownPartitionContainsRightBoundary() const;

  //! get the rank no of the left neighbouring rank
  int leftNeighbourRankNo() const;

  //! get the rank no of the right neighbouring rank
  int rightNeighbourRankNo() const;

  //! get the rank no of the top neighbouring rank
  int topNeighbourRankNo() const;

  //! get the rank no of the bottom neighbouring rank
  int bottomNeighbourRankNo() const;


  //! get the offset values for counting local nodes in x and y direction. 
  //! (i_local,j_local) + nodeOffset = (i_global,j_global)
  //! used in OutputWriterParaviewParallel
  std::array<int,2> nodeOffset() const;

  /**
   * @brief Return whether this corner is red or black
  */
  bool lowerLeftIsRed();

  /**
   * @brief processes in x and y direction
  */
  std::array<int,2> nProcesses();

  /**
   * @brief decides whether it first sends or first receives horizontally
  */
  bool sendsFirstUpDown();

  /**
   * @brief decides whether it first sends or first receives vertically
  */
 bool sendsFirstLeftRight();




  private:
    void calcNodeOffset();
    void calcLowerLeftIsRed();
    std::array<int,2> findOptimumProcessAlignment();
    std::array<int,2> calculateNCellsLocal();


    std::shared_ptr<Communicator> communicator_;

    const std::array<int,2> nCellsGlobal_;

    std::array<int,2> nCellsLocal_;

    std::array<int,2> ownProcess_;
    std::array<int,2> nProcesses_;

    std::array<int,2> remainderLocalCells_;


    const int ownRankNo_;
    const int nRanks_;

    bool lowerLeftIsRed_;
    std::array<int,2> nodeOffset_;

    int bottomNeighbourRankNo_;
    int topNeighbourRankNo_;
    int leftNeighbourRankNo_;
    int rightNeighbourRankNo_;

    bool containsBottomBoundary_;
    bool containsTopBoundary_;
    bool containsLeftBoundary_;
    bool containsRightBoundary_;


    // std::array<int,2> remainderLocalCells_;

    void printDebugInformation();
};


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
  /**
   * @brief Constructor
   *
   * @param nCellsGlobal the number of overall cells in domain
   * @param communicator Communicator for communication with neighbouring processes
   */
  Partitioning(std::array<int, 2> nCellsGlobal,
               std::shared_ptr<Communicator> communicator);

  /**
   * @brief return the number of cells in subdomain
   */
  std::array<int, 2> nCellsLocal() const;

  /**
   * @brief return the number of cells in full domain 
   */
  std::array<int, 2> nCellsGlobal() const;

  /**
   * @brief return the number of the own rank 
   */
  int ownRankNo() const;

  /**
   * @brief return the total number of ranks 
   */
  int nRanks() const;

  /**
   * @brief return true if partition contains bottom boundary 
   */
  bool ownPartitionContainsBottomBoundary() const;

  /**
   * @brief return true if partition contains top boundary 
   */
  bool ownPartitionContainsTopBoundary() const;

  /**
   * @brief return true if partition contains left boundary 
   */
  bool ownPartitionContainsLeftBoundary() const;

  /**
   * @brief return true if partition contains right boundary 
   */
  bool ownPartitionContainsRightBoundary() const;

  /**
   * @brief rank number of left neighbour 
   */
  int leftNeighbourRankNo() const;

  /**
   * @brief rank number of right neighbour 
   */
  int rightNeighbourRankNo() const;

  /**
   * @brief rank number of top neighbour 
   */
  int topNeighbourRankNo() const;

  /**
   * @brief rank number of bottom neighbour 
   */
  int bottomNeighbourRankNo() const;

  /**
   * @brief the offset of the subdomain in cells relative to the global coordinate system
   */
  std::array<int, 2> nodeOffset() const;

  /**
   * @brief Return whether this lower left corner is red or black in the global coordinate system
   */
  bool lowerLeftIsRed();

  /**
   * @brief global processes in x and y direction
   */
  std::array<int, 2> nProcesses();

private:
  void calcNodeOffset();
  void calcLowerLeftIsRed();
  std::array<int, 2> findOptimumProcessAlignment();
  std::array<int, 2> calculateNCellsLocal();

  std::shared_ptr<Communicator> communicator_;

  const std::array<int, 2> nCellsGlobal_;

  std::array<int, 2> nCellsLocal_;

  std::array<int, 2> ownProcess_;
  std::array<int, 2> nProcesses_;

  std::array<int, 2> remainderLocalCells_;

  const int ownRankNo_;
  const int nRanks_;

  bool lowerLeftIsRed_;
  std::array<int, 2> nodeOffset_;
};

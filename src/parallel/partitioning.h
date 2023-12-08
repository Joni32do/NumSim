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
   * @brief resturns the offset of the subdomain in cells relative to the global coordinate system
   */
  std::array<int, 2> nodeOffset() const;

  /**
   * @brief calculates whether this corner is red or black
   */
  bool lowerLeftIsRed();

  /**
   * @brief processes in x and y direction
   */
  std::array<int, 2> nProcesses();

private:
  /**
   * @brief calculates the offset of the subdomain in cells relative to the global coordinate system
   */
  void calcNodeOffset();

  /**
   * @brief returns whether this corner is red or black
   */
  void calcLowerLeftIsRed();

  /**
   * @brief calculates the optimal aligment of processed in domain
   *
   * Uses the minization of bondary between subprocesses as optimization goal
   */
  std::array<int, 2> findOptimumProcessAlignment();

  /**
   * @brief calculates the number of cells in subdomain
   */
  std::array<int, 2> calculateNCellsLocal();

  std::shared_ptr<Communicator> communicator_; //!< comminucator instance

  const std::array<int, 2> nCellsGlobal_; //!< number of global cells

  std::array<int, 2> nCellsLocal_; //!< number of local cells

  std::array<int, 2> ownProcess_; //!< positon of subdomain in domain with i, j index
  std::array<int, 2> nProcesses_; //!< subprocesses in x and y direction

  std::array<int, 2> remainderLocalCells_; //!< number of subdomains that are bigger by one cell than the other ones in x and y direction

  const int ownRankNo_; //!< rank number of process
  const int nRanks_;    //!< total number of ranks

  bool lowerLeftIsRed_;           //!< bool whether lower left cell is in red or black iteration in solver
  std::array<int, 2> nodeOffset_; //!< node offset relative to the global coordinate system

  int bottomNeighbourRankNo_; //!< rank number of bottom neighbour
  int topNeighbourRankNo_;    //!< rank number of top neighbour
  int leftNeighbourRankNo_;   //!< rank number of left neighbour
  int rightNeighbourRankNo_;  //!< rank number of right neighbour

  bool containsBottomBoundary_; //!< bool whether bottom is boundary
  bool containsTopBoundary_;    //!< bool whether top is boundary
  bool containsLeftBoundary_;   //!< bool whether left is boundary
  bool containsRightBoundary_;  //!< bool whether right is boundary
};

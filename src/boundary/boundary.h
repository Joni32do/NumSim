#pragma once

#include "mask.h"
#include "../discretization/discretization.h"
#include "../settings_parser/settings.h"
#include <vector>
#include <memory>

/**
 * @class Boundary
 * @brief Class responsible to caluclate boundary conditions
 *
 */
class Boundary
{
public:
    /**
     * @brief Constructor.
     *
     * @param discretization instance of Discretization holding the needed field variables for rhs and p
     * @param mask instance of mask holding the boundary condition enums for each cell tolerance for the solver
     * @param settings holding the necessary domainn boundary condition
     */
    Boundary(std::shared_ptr<Mask> mask, std::shared_ptr<Discretization> discretization, Settings settings);

    /**
     * @brief calculates and sets the pressure boundary values for obstacles and domain boundary
     */
    void setPressureBoundaryValues();

    /**
     * @brief calculates and sets the pressure boundary values for surface boundary
     *
     * Only needs to be set once -> no need to update each solver iteration
     */
    void setPressureSurfaceBC();

    /**
     * @brief calculates and sets the velocity boundary values for surface, obstacle and domain boundary
     */
    void setVelocityBoundaryValues();

    /**
     * @brief overloaded function that sets velocity boundary
     *
     * necessary when fluid tracker is used
     *
     * @param dt current time step
     */
    void setVelocityBoundaryValues(double dt);

    /**
     * @brief returns true if cell left and rigt of F value are fluid (not a boundary value)
     */
    bool doCalculateF(int i, int j) const;

    /**
     * @brief returns true if cell top and bottom of G value are fluid (not a boundary value)
     */
    bool doCalculateG(int i, int j) const;

    /**
     * @brief updates fluid boundary cell list
     */
    void updateBoundary();

    std::shared_ptr<Mask> mask_; //!< shared pointer to mask entity

private:
    /**
     * @brief creates 3 lists for all fluid, obstacle and domain boundary cells
     */
    void createBoundaryCellsLists();

    /**
     * @brief Set pressure values at domain boundary
     */
    void setPressureDomainBC();

    /**
     * @brief Set pressure values at obstacle boundary
     */
    void setPressureObstacleBC();

    /**
     * @brief Set velocity values at domain boundary
     */
    void setVelocityDomainBC();

    /**
     * @brief Set velocity values at obstacle boundary
     */
    void setVelocityObstacleBC();

    /**
     * @brief Set velocity values at surface boundary
     */
    void setVelocitySurfaceBC();

    /**
     * @brief Update velocity boundary conditions at surface
     */
    void updateVelocitySurfaceBC(double dt);

    /**
     * @brief set values in dirscretization for all air cells to zero
     */
    void setAirCellsZero();

    std::shared_ptr<Discretization> discretization_; //!< shared pointer to discretization entity
    Settings settings_;                              //!< settings loaded from settings file

    std::vector<int> domainBoundaryCells_;   //!< indice list of all domain boundary cells
    std::vector<int> obstacleBoundaryCells_; //!< indice list of all obstacle boundary cells
    std::vector<int> fluidBoundaryCells_;    //!< indice list of all fluid boundary cells
};
#pragma once

#include "pressure_solver.h"

/**
 * @class SOR
 * @brief Successive over-relaxation solver
 *
 */
class SOR : public PressureSolver
{

public:
    /**
     * @brief Constructor.
     *
     * @param seettings contains settings for solver
     */
    SOR(const std::shared_ptr<Discretization> &data,
        Settings settings);

    /**
     * @brief override function that starts solver.
     *
     */
    void solve() override;
};
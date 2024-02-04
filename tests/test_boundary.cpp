#include <gtest/gtest.h>
#include "../src/boundary/boundary.h"
#include "../src/discretization/central_differences.h"

TEST(Boundary, BoundaryCellsWithoutObstacle) {
    Settings settings;
    settings.nCells = {10, 10};
    settings.obstaclePosition = {0.45, 0.45};
    settings.obstacleDimension = {0.1, 0.1};
    settings.physicalSize = {1.0, 1.0};
    std::array<double, 2> meshWidth = {settings.physicalSize[0] / settings.nCells[0], settings.physicalSize[1] / settings.nCells[1]};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);

    Boundary boundary(mask, discretization, settings);

};

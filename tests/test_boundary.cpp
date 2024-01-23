#include <gtest/gtest.h>
#include "../src/boundary/boundary.h"
#include "../src/discretization/central_differences.h"

TEST(Boundary, BoundaryCellsWithoutObstacle) {
    std::array<int,2> n_cells = {4,3};
    std::array<double,2> meshWidth = {1.0, 1.0};
    std::shared_ptr<Mask> mask = std::make_shared<Mask>(n_cells);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(n_cells, meshWidth);
    Settings settings;
    Boundary boundary(mask, discretization, settings);

    EXPECT_EQ(boundary.getNumberOfBoundaryCells(), 18);
};

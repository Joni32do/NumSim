#include <gtest/gtest.h>
#include "../src/boundary/boundary.h"
#include "../src/discretization/central_differences.h"

TEST(Boundary, BoundaryCellsWithoutObstacle) {
    Settings settings;
    settings.nCells = {10, 10};
    settings.physicalSize = {1.0, 1.0};
    std::array<double, 2> meshWidth = {settings.physicalSize[0] / settings.nCells[0], settings.physicalSize[1] / settings.nCells[1]};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);

    Boundary boundary(mask, discretization, settings);
};


TEST(Boundary, BoundaryCellsWithObstacle) {
    Settings settings;
    settings.nCells = {10, 10};
    settings.obstaclePosition = {0.45, 0.45};
    settings.obstacleDimension = {0.1, 0.1};
    settings.physicalSize = {1.0, 1.0};
    std::array<double, 2> meshWidth = {settings.physicalSize[0] / settings.nCells[0], settings.physicalSize[1] / settings.nCells[1]};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    mask->makeRectangularObstacle();
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);

    Boundary boundary(mask, discretization, settings);

};

TEST(Boundary, SurfacePressure) {
    // Reset this grid with all 16 possibilities

    // A A A
    // F F F
    // F F A

    // (sourrounded by domain)

    Settings settings;
    settings.nCells = {3, 3};
    settings.physicalSize = {1.0, 1.0};
    std::array<double, 2> meshWidth = {settings.physicalSize[0] / settings.nCells[0], settings.physicalSize[1] / settings.nCells[1]};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);
    Boundary boundary(mask, discretization, settings);
    for (int i = 0; i < 16; i++) {
        mask->resetMask();
        if (i%2 > 0){
            (*mask)(1, 0) = Mask::FLUID;
        }
        if (i%4 > 2){
            (*mask)(1, 1) = Mask::FLUID;
        }
        if (i%8 > 4){
            (*mask)(1, 2) = Mask::FLUID;
        }
        if (i%16 > 8){
            (*mask)(0, 1) = Mask::FLUID;
        }

        // boundary.setSurfacePressure();
        // std::cout << "Case " << i << "\n";
        // mask_->printMask();

        
    }

}
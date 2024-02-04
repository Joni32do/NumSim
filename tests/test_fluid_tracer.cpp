#include <gtest/gtest.h>
#include "../src/surface/fluid_tracer.h"
#include "../src/discretization/central_differences.h"

// Experimental stuff
#include <unistd.h>

TEST(FluidTracer, NumberParticlesSingleCell){
    /*
    Test for a single cell (with 8 boundary cells) how many particles are placed
    */
    Settings settings;
    settings.nCells = {1, 1};
    settings.physicalSize = {1.0, 1.0};
    std::array<double,2> meshWidth = {1.0, 1.0};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);


    int numParticlesPerCell = 1;
    FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);
    EXPECT_GE(fluid_tracer.getNumberOfParticles(), numParticlesPerCell);

    int numParticlesPerCell2 = 9;
    FluidTracer fluid_tracer2(numParticlesPerCell2, discretization, mask);
    EXPECT_GE(fluid_tracer2.getNumberOfParticles(), numParticlesPerCell2);

    int numParticlesPerCell3 = 17;
    FluidTracer fluid_tracer3(numParticlesPerCell3, discretization, mask);
    EXPECT_GE(fluid_tracer3.getNumberOfParticles(), numParticlesPerCell3);

    int numParticlesPerCell4 = 100;
    FluidTracer fluid_tracer4(numParticlesPerCell4, discretization, mask);
    EXPECT_GE(fluid_tracer4.getNumberOfParticles(), numParticlesPerCell4);
    EXPECT_GE(fluid_tracer4.getNumberOfParticles(), fluid_tracer3.getNumberOfParticles());
    EXPECT_LE(fluid_tracer4.getNumberOfParticles(), 2 * numParticlesPerCell4);
};

TEST(FluidTracer, DistributionSingleCell){
    /*
    Test for a single cell (with 8 boundary cells) how many particles are placed
    */
    Settings settings;
    settings.nCells = {1, 1};
    settings.physicalSize = {1.0, 1.0};
    std::array<double,2> meshWidth = {1.0, 1.0};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);


    int numParticlesPerCell = 1;
    FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);
    EXPECT_NEAR(fluid_tracer.getParticlePosition(0)[0], 0.5, 0.001);
    EXPECT_NEAR(fluid_tracer.getParticlePosition(0)[1], 0.5, 0.001);

    int numParticlesPerCell2 = 9;
    FluidTracer fluid_tracer2(numParticlesPerCell2, discretization, mask);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(0)[0], 0.166667, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(0)[1], 0.166667, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(1)[0], 0.5, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(1)[1], 0.166667, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(4)[0], 0.5, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(4)[1], 0.5, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(8)[0], 0.833333, 0.001);
    EXPECT_NEAR(fluid_tracer2.getParticlePosition(8)[1], 0.833333, 0.001);

    int numParticlesPerCell3 = 17;
    FluidTracer fluid_tracer3(numParticlesPerCell3, discretization, mask);
    EXPECT_NEAR(fluid_tracer3.getParticlePosition(0)[0], 0.125, 0.1);
    EXPECT_NEAR(fluid_tracer3.getParticlePosition(0)[1], 0.1, 0.1);
    EXPECT_NEAR(fluid_tracer3.getParticlePosition(1)[0], 0.375, 0.1);
    EXPECT_NEAR(fluid_tracer3.getParticlePosition(1)[1], 0.1, 0.1);

}


TEST(FluidTracer, val2CellXAndY){
    Settings settings;
    settings.nCells = {1, 1};
    settings.physicalSize = {1.0, 1.0};
    std::array<double,2> meshWidth = {1.0, 1.0};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);

    FluidTracer fluid_tracer(1, discretization, mask);

    EXPECT_EQ(fluid_tracer.val2CellX(0.5), 1);
    EXPECT_EQ(fluid_tracer.val2CellX(0.0), 1);
    EXPECT_EQ(fluid_tracer.val2CellX(-0.5), 0);
    EXPECT_EQ(fluid_tracer.val2CellX(1.5), 2);

    EXPECT_EQ(fluid_tracer.val2CellY(0.5), 1);
    EXPECT_EQ(fluid_tracer.val2CellY(0.0), 1);
    EXPECT_EQ(fluid_tracer.val2CellY(-0.5), 0);
    EXPECT_EQ(fluid_tracer.val2CellY(1.5), 2);
}

TEST(FluidTracer, moveParticles_1Cell_1Particle_WithoutCollision){
    //       u(0,2)  -----   u(1,2) 
    //         :               :
    // v(0,1)  |____v(1,1)_____|  v(2,1)
    //         |               |
    //       u(0,1)  -----   u(1,1)
    //         |               |
    // v(0,0)  |____v(1,0)_____|  v(2,0)
    //         :               :
    //       u(0,0)  -----   u(1,0)
    //
    
    Settings settings;
    settings.nCells = {1, 1};
    settings.physicalSize = {1.0, 1.0};
    std::array<double,2> meshWidth = {1.0, 1.0};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);
    int numParticlesPerCell = 1;
    FluidTracer fluid_tracer(numParticlesPerCell, discretization, mask);

    std::array<double, 7> velocitiesU = {-1, -0.1, -0.01, 0, 0.01, 0.1, 1};
    std::array<double, 7> velocitiesV = {-1, -0.1, -0.01, 0, 0.01, 0.1, 1};
    
    // Checks 49 Cases
    for (int idx_u = 0; idx_u < velocitiesU.size(); idx_u++){
        for (int idx_v = 0; idx_v < velocitiesV.size(); idx_v++){

            // Set all velocities to the same value
            discretization->u(0,0) = velocitiesU[idx_u];
            discretization->u(1,0) = velocitiesU[idx_u];
            discretization->u(0,1) = velocitiesU[idx_u];
            discretization->u(1,1) = velocitiesU[idx_u];
            discretization->u(0,2) = velocitiesU[idx_u];
            discretization->u(1,2) = velocitiesU[idx_u];

            discretization->v(0,0) = velocitiesV[idx_v];
            discretization->v(1,0) = velocitiesV[idx_v];
            discretization->v(2,0) = velocitiesV[idx_v];
            discretization->v(0,1) = velocitiesV[idx_v];
            discretization->v(1,1) = velocitiesV[idx_v];
            discretization->v(2,1) = velocitiesV[idx_v];

            // Initialize Fluid tracer with single particle
            FluidTracer fluid_tracer(1, discretization, mask);
            
            double old_pos_x = fluid_tracer.getParticlePosition(0)[0];
            double old_pos_y = fluid_tracer.getParticlePosition(0)[1];
            EXPECT_EQ(fluid_tracer.getNumberOfParticles(), 1);
            EXPECT_EQ(old_pos_x, 0.5);
            EXPECT_EQ(old_pos_y, 0.5);

            // Update particle according to velocity

            double dt = 0.1; // <! ensures that particle does not leave cell>
            fluid_tracer.moveParticles(dt);
            EXPECT_EQ(fluid_tracer.getParticlePosition(0)[0], old_pos_x + dt * velocitiesU[idx_u]);
            EXPECT_EQ(fluid_tracer.getParticlePosition(0)[1], old_pos_y + dt * velocitiesV[idx_v]);
        }
    }
}


TEST(FluidTracer, moveParticles_1Cell_1Particle_WithCollision){
    //       u(0,2)  -----   u(1,2) 
    //         :               :
    // v(0,1)  |____v(1,1)_____|  v(2,1)
    //         |               |
    //       u(0,1)  -----   u(1,1)
    //         |               |
    // v(0,0)  |____v(1,0)_____|  v(2,0)
    //         :               :
    //       u(0,0)  -----   u(1,0)
    //

    // TODO: check obstacle edges
    
    Settings settings;
    settings.nCells = {1, 1};
    settings.physicalSize = {1.0, 1.0};
    std::array<double,2> meshWidth = {1.0, 1.0};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);

    std::array<double, 4> velocitiesU = {-6, -1, 1, 6};
    std::array<double, 4> velocitiesV = {-6, -1, 1, 6};
    // std::array<double, 1> velocitiesU = {8};
    // std::array<double, 1> velocitiesV = {1};
    
// Checks 16 Cases --> 0°, 27°, 45°, 63°, 90°, ...
    for (int idx_u = 0; idx_u < velocitiesU.size(); idx_u++){
        for (int idx_v = 0; idx_v < velocitiesV.size(); idx_v++){

            // Set all velocities to the same value
            discretization->u(0,0) = velocitiesU[idx_u];
            discretization->u(1,0) = velocitiesU[idx_u];
            discretization->u(0,1) = velocitiesU[idx_u];
            discretization->u(1,1) = velocitiesU[idx_u];
            discretization->u(0,2) = velocitiesU[idx_u];
            discretization->u(1,2) = velocitiesU[idx_u];

            discretization->v(0,0) = velocitiesV[idx_v];
            discretization->v(1,0) = velocitiesV[idx_v];
            discretization->v(2,0) = velocitiesV[idx_v];
            discretization->v(0,1) = velocitiesV[idx_v];
            discretization->v(1,1) = velocitiesV[idx_v];
            discretization->v(2,1) = velocitiesV[idx_v];


            // Initialize Fluid tracer with single particle
            FluidTracer fluidTracer(1, discretization, mask);

            double old_pos_x = fluidTracer.getParticlePosition(0)[0];
            double old_pos_y = fluidTracer.getParticlePosition(0)[1];
            EXPECT_EQ(fluidTracer.getNumberOfParticles(), 1);
            EXPECT_EQ(old_pos_x, 0.5);
            EXPECT_EQ(old_pos_y, 0.5);
            
            // Update particle
            double dt = 0.1; 
            fluidTracer.moveParticles(dt);

            // Particle is still in the box
            EXPECT_GE(fluidTracer.getParticlePosition(0)[0], 0.0);
            EXPECT_LE(fluidTracer.getParticlePosition(0)[0], 1.0);
            EXPECT_GE(fluidTracer.getParticlePosition(0)[1], 0.0);
            EXPECT_LE(fluidTracer.getParticlePosition(0)[1], 1.0);


            // CHECK A SPECIAL CASE 
            // if (idx_u == 3 && idx_v == 2){
            //     // Particles position is
            //     double new_pos_x = 0.75;
            //     double new_pos_y = 0.875;
            //     EXPECT_EQ(fluid_tracer.getParticlePosition(0)[0], new_pos_x);
            //     EXPECT_EQ(fluid_tracer.getParticlePosition(0)[1], new_pos_y);
            // }
        }
    }
}


TEST(FluidTracer, moveParticlesInLargeGrid){
    // Flow Field like this:
    //
    // Right Down
    // Up    Left
    // 
    // Or larger
    //
    // R R R D D D
    // R R R D D D
    // R R R D D D
    // U U U L L L
    // U U U L L L
    // U U U L L L

    Settings settings;
    settings.nCells = {9, 9};
    settings.physicalSize = {1.0, 1.0};
    std::array<double,2> meshWidth = {settings.physicalSize[0] / settings.nCells[0], settings.physicalSize[1] / settings.nCells[1]};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);

    std::vector<double> x = {0.1, 0.1, 0.9, 0.9, 0.25, 0.75, 0.25, 0.75};
    std::vector<double> y = {0.1, 0.9, 0.1, 0.9, 0.25, 0.25, 0.75, 0.75};
    FluidTracer fluidTracer(x, y, discretization, mask);


    // Create an interesting flow-field: Wirbel
    for (int i = 0; i < settings.nCells[0]; i++){
        for (int j = 0; j < settings.nCells[1]; j++){
            if (i > settings.nCells[0]/2){
                if (j > settings.nCells[1]/2){
                    discretization->v(i,j) = -1;
                } else {
                    discretization->u(i,j) = -1;
                }
            } else {
                if (j > settings.nCells[1]/2){
                    discretization->u(i,j) = 1;
                } else {
                    discretization->v(i,j) = 1;
                }
            }
        }
    }
    // Move particles for 100 steps
    double dt = 0.1;
    for (int k = 0; k < 100; k++){
        fluidTracer.moveParticles(dt);


        // particles are still in the box
        for (int i = 0; i < fluidTracer.getNumberOfParticles(); i++){
            EXPECT_GE(fluidTracer.getParticlePosition(i)[0], 0.0);
            EXPECT_LE(fluidTracer.getParticlePosition(i)[0], 1.0);
            EXPECT_GE(fluidTracer.getParticlePosition(i)[1], 0.0);
            EXPECT_LE(fluidTracer.getParticlePosition(i)[1], 1.0);
        }
    }

}

TEST(FluidTracer, moveDistributedParticlesInLargeGrid){
    // Flow Field like this:
    //
    // Right Down
    // Up    Left
    // 
    // Or larger
    //
    // R R R D D D
    // R R R D D D
    // R R R D D D
    // U U U L L L
    // U U U L L L
    // U U U L L L

    Settings settings;
    settings.nCells = {9, 9};
    settings.physicalSize = {1.0, 1.0};
    std::array<double,2> meshWidth = {settings.physicalSize[0] / settings.nCells[0], settings.physicalSize[1] / settings.nCells[1]};
    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);

    FluidTracer fluidTracer(1, discretization, mask);
    double vel = meshWidth[0];
    for (int i = 0; i < settings.nCells[0]; i++){
        for (int j = 0; j < settings.nCells[1] + 1; j++){
            // Flow to the right
            discretization->u(i,j) = vel;
        }
    }

    double dt = 0.5;
    for (int k = 0; k < 1; k++){
        fluidTracer.moveParticles(dt);
        mask->printMask();


        // particles are still in the box
        for (int i = 0; i < fluidTracer.getNumberOfParticles(); i++){
            EXPECT_GE(fluidTracer.getParticlePosition(i)[0], 0.0);
            EXPECT_LE(fluidTracer.getParticlePosition(i)[0], 1.0);
            EXPECT_GE(fluidTracer.getParticlePosition(i)[1], 0.0);
            EXPECT_LE(fluidTracer.getParticlePosition(i)[1], 1.0);
        }

    }

}

// Benötigt Test Methode mit return velocity
TEST(FluidTracer, DVDLogoBounceOff){
    
    Settings settings;
    settings.nCells = {10, 10};
    settings.obstaclePosition = {0.45, 0.45};
    settings.obstacleDimension = {0.1, 0.1};
    settings.physicalSize = {1.0, 1.0};
    std::array<double, 2> meshWidth = {settings.physicalSize[0] / settings.nCells[0], settings.physicalSize[1] / settings.nCells[1]};

    std::shared_ptr<Mask> mask = std::make_shared<Mask>(settings);
    std::shared_ptr<Discretization> discretization = std::make_shared<CentralDifferences>(settings.nCells, meshWidth);

    mask->makeRectangularObstacle();
    mask->setObstacleBC();

    // Initial values
    std::array<double, 2> vel = {0.35*meshWidth[0], -0.5 * meshWidth[1]};
    double dt = 1;
    std::vector<double> x = {0.5};
    std::vector<double> y = {0.2};

    FluidTracer fluidTracer(x, y, discretization, mask);


    for (int k = 0; k < 100; k++){

        
        mask->printMask();
        std::cout << k << " ";
        vel = fluidTracer.moveParticles(dt, vel);
        std::cout << "\033[2J\033[1;1H";
        usleep(10000);

    }
}



// -----------------------------------------------------------------
// --- T E S T    U P D A T E    M A S K    B O U N D A R I E S ---
// -----------------------------------------------------------------



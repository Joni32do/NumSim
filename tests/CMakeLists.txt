cmake_minimum_required(VERSION 3.14)

# 'Google_test' is the subproject name
project(tests)

# 'lib' is the folder with Google Test sources
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# 'Google_Tests_run' is the target name
# 'test1.cpp test2.cpp' are source files with tests
add_executable(run_tests 
    test_array2D.cpp
    test_field_variable.cpp
    test_staggered_grid.cpp
    test_donor_cell.cpp
    test_central_differences.cpp
    test_mask.cpp
    test_boundary.cpp
    test_fluid_tracer.cpp
    ../src/storage/array2D.cpp
    ../src/storage/field_variable.cpp
    ../src/discretization/staggered_grid.cpp
    ../src/discretization/discretization.cpp
    ../src/discretization/donor_cell.cpp
    ../src/discretization/central_differences.cpp
    ../src/boundary/mask.cpp
    ../src/boundary/boundary.cpp
    ../src/surface/fluid_tracer.cpp
)


find_package(PNG REQUIRED)

target_link_libraries(run_tests gtest gtest_main ${PNG_LIBRARIES})

# Set the version of the C++ standard to use, we use C++17, published in 2014
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
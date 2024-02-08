#!/bin/bash
# Execute with `./build.sh` in the terminal, with
# an optional input parameter "release" for no debug info
# You may need to allow execution with
#    `chmod +x build.sh`

rm -rf build

mkdir -p build
cd build

if [[ "${1,,}" == "release" ]]; then
	cmake -DCMAKE_BUILD_TYPE=Release ..
else
	cmake -DCMAKE_BUILD_TYPE=Debug ..
fi

make install -j4


./numsim ../input/settings.txt
# ./numsim ../input/settings_aerodynamic_obstacle.txt
# ./numsim ../input/settings_damBreak.txt
# ./numsim ../input/settings_drop.txt
# ./numsim ../input/settings_quadruple_cavity.txt
# ./numsim ../input/settings_lid_driven_cavity_with_a_step.txt

# ./numsim ../input/settings_water_tap.txt


cd ..

# Documentation

# mkdir documentation
# cd documentation
# doxygen ../Doxyfile
# cd ..

# Tests
build/tests/run_tests
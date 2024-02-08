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


# ./numsim ../input/settings.txt
./numsim ../input/settings_aerodynamic_obstacle.txt
# ./numsim ../input/settings_damBreak.txt
# ./numsim ../input/settings_drop.txt
# ./numsim ../input/settings_quadruple_cavity.txt
# ./numsim ../input/settings_lid_driven_cavity_with_a_step.txt

# ./numsim ../input/settings_debug_joni.txt # maze
# ./numsim ../input/settings_water_tap.txt

######################
## DEBUG -> DELETE ##
######################
dir="/home/jonat/Documents/02_Workspace/05_NumSim_misc/Project/Simulations/data"

# rm -rf $dir/damBreak/slow
# mkdir $dir/damBreak/slow
# cp -r out $dir/damBreak/slow


# ./numsim ../input/settings_drop.txt
# newdir="dropTheCometHard"
# rm -rf $dir/$newdir
# mkdir $dir/$newdir
# cp -r out $dir/$newdir/



# ./numsim ../input/bitmap/doubleDam/doubleDam.txt
# rm -rf "$dir"/doubleDam/*
# cp -r out "$dir"/doubleDam/

# ./numsim ../input/bitmap/doubleDam/doubleDamLeft.txt
# rm -rf "$dir"/doubleDamLeft/*
# cp -r out "$dir"/doubleDamLeft/

# ./numsim ../input/bitmap/doubleDam/doubleDamRight.txt
# rm -rf "$dir"/doubleDamRight/*
# cp -r out "$dir"/doubleDamRight/

# ./numsim ../input/bitmap/doubleDam/doubleDamTop.txt
# rm -rf "$dir"/doubleDamTop/*
# cp -r out "$dir"/doubleDamTop/out


# ./numsim ../input/settings_debug_joni.txt


########################
cd ..

# directly open output in paraview

# paraview out/output_*

# for writing documentation

# cd ..
# mkdir documentation
# cd documentation
# doxygen ../Doxyfile
# cd ..

# run tests
#build/tests/run_tests

# zip -r submission.zip src/ CMakeLists.txt
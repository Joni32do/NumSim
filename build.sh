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
# ./numsim ../input/settings_v3.txt
./numsim ../input/different_resolutions/settings_01.txt
./numsim ../input/different_resolutions/settings_02.txt
./numsim ../input/different_resolutions/settings_03.txt
./numsim ../input/different_resolutions/settings_04.txt
./numsim ../input/different_resolutions/settings_05.txt
./numsim ../input/different_resolutions/settings_06.txt
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
# build/tests/run_tests

zip -r submission.zip src/ CMakeLists.txt
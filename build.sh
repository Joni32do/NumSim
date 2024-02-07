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

# ./numsim ../input/settings_debug_joni.txt
./numsim ../input/settings.txt
./numsim ../input/settings_v3.txt
# ./numsim ../input/settings_damBreak.txt
# ./numsim ../input/settings_drop.txt



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
build/tests/run_tests

# zip -r submission.zip src/ CMakeLists.txt
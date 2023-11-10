# This is for lazy people -> just type 
#   `./build.sh` 
# in the terminal
#
# You may need to allow execution
#    `chmod +x build.sh`

# rm -rf build

mkdir -p build
cd build
cmake ..
make
src/numsim ../input/settings.txt
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

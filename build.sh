# This is for lazy people -> just type 
#   `./build.sh` 
# in the terminal
#
# You may need to allow execution
#    `chmod +x build.sh`
# rm -rf build
if ["$1" == "clean"]; then
    rm -rf build
    rm -rf resources
fi

mkdir -p build
cd build
cmake ..
make
src/numsim ../settings.txt

# directly open output in paraview
# paraview out/output_*

# for writing documentation

# cd ..
# mkdir resources
# cd resources
# doxygen ../Doxyfile
# cd ..

# run tests
build/tests/run_tests

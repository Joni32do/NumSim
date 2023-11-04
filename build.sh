# This is for lazy people -> just type `./build.sh` in the terminal
rm -rf build
mkdir -p build
cd build
cmake ..
make
src/numsim ../settings.txt
tests/run_tests
#paraview out/output_*  # or manually open in paraview
# You may need to change access with `chmod +x build.sh`

#cmake -D CMAKE_PREFIX_PATH "home/.."
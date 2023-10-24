# This is for lazy people -> just type `./build.sh` in the terminal
mkdir -p build
cd build
cmake ..
make
src/numsim ../settings.txt
paraview out/output_*  # or manually open in paraview
# You may need to change access with `chmod +x build.sh`
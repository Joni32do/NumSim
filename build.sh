#!/bin/bash
# Execute with `./build.sh` in the terminal, with
# an optional input parameter "release" for no debug info
# You may need to allow execution with
#    `chmod +x build.sh`##

module use /usr/local.nfs/sgs/modulefiles
module load gcc/10.2
module load openmpi/3.1.6-gcc-10.2
module load vtk/9.0.1
module load cmake/3.18.2

# enable custom build openmpi 3.1 that works with slurm
export CPATH=/scratch-nfs/maierbn/openmpi/install-3.1/include
export PATH=/scratch-nfs/maierbn/openmpi/install-3.1/bin:$PATH


rm -rf src/debugging/debug_out
mkdir src/debugging/debug_out

rm -rf build
mkdir -p build
cd build

if [[ "${1,,}" == "release" ]]; then
	cmake -DCMAKE_BUILD_TYPE=Release ..
else
	cmake -DCMAKE_BUILD_TYPE=Debug ..
fi

make install -j4

time srun -n 16 ./numsim_parallel ../input/settings.txt

cd ..
rm output.zip
zip -qr output.zip build/out/ src/debugging/debug_out/


rm submission.zip
zip -qr submission.zip src/ CMakeLists.txt

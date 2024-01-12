#! /bin/bash

module load sgscl1/openfoam/2306

rm -r 0.* 1* 2* 3* 4* 5* 6* 7* 8* 9* processor* postProcessing

blockMesh
decomposePar
srun -n 6 icoFoam -parallel
reconstructPar

rm result.zip

zip -r result.zip *


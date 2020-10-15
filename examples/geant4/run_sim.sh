#!/bin/sh

mkdir -p data
cd data
o2-sim -n 5
o2-sim-digitizer-workflow -b --onlyDet=TRD

#!/bin/bash
# Create URDF and SDF files from xacro

cd ../urdf
echo "Exporting URDF..."
rosrun xacro xacro.py quad.urdf.xacro  > ../robots/quad.urdf
echo "Exporting SDF..."
gzsdf print ../robots/quad.urdf > ../robots/quad.sdf

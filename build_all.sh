#bin/sh

rm -rf build
mkdir build
cd build

#cmake ..
#make all

cmake -G"Xcode" ..
open HelloOpenGL.xcodeproj

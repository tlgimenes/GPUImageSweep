GPUImageSweep
=============

Introduction
------------

This code is an Open{CV,CL,GL} implementation of the plan sweep algorithm. For
comparation purposes a cpu implementation showing a gray contrast intead of a 
3D model was made.

Install
------------

This program uses cmake, but is supported in Linux only. If you want to be
a Windows tester, please mail the developpers.

*Dependencies:*
- OpenCL 1.1 drivers with headers
- OpenGL 
- OpenCV
- GLUT
- GLEW

**Linux:**

*Compiling:*

Open a terminal and type:

    cd GPUImageSweep

to go to the source code's directory 

    mkdir release

to create a folder where the binary will be placed

    cd release

to get in the folder

    cmake -DCMAKE_BUILD_TYPE="Release"

to configure the build

    make

to compile the code

You should be good to go now

**Windows:**

It should work well the compiling part, but the code was never tested on windows, so good luck !

Usage
------------

After compiling you should have now to binary files, one called gpuplansweep
and the other one called cpuPlanSweep. For running both just type:

    ./gpuplansweep
    
    ./cpuPlanSweep

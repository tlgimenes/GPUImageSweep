# Minimum cmake version
cmake_minimum_required(VERSION 2.6)

# Project name
project(GPUPlanSweep)

# Internal files
set(SRC main.cpp)
set(INC clgl/include image/include windowManager/include)
set(SUBFOLDERS clgl image windowManager)
set(C_FLAGS -std=c++11)

# Compiler flags
add_definitions(${C_FLAGS})

# Internal header files
include_directories(${INC})
subdirs(${SUBFOLDERS})

# This is only required for the script to work in the version control
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

# Required external packages
find_package(OpenGL REQUIRED)
find_package(GLUT   REQUIRED)
find_package(GLEW   REQUIRED)
find_package(OpenCL REQUIRED)
find_package(OpenCV REQUIRED)

include_directories( ${OPENCL_INCLUDE_DIRS} )

# Link internal files
add_executable(gpuplansweep ${SRC})

# Link necessary libraries
target_link_libraries(gpuplansweep 
                     ${OpenCV_LIBS} 
                     ${OpenGL_LIBS}
                     ${GLUT_LIBS}
                     ${GLEW_LIBS}
                     ${OPENCL_LIBRARIES}
                     )

target_link_libraries(gpuplansweep                     
                      clgl
                      image
                      windowManager
                     )


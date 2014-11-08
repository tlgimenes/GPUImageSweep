# Minimum cmake version
cmake_minimum_required(VERSION 2.6)

# Libary name
set(LIB_NAME planeSweep)

# Internal files for this module
set(SRC homography.cpp plane_sweep.cpp)
set(C_FLAGS -std=c++11)

# Compiler flags
add_definitions(${C_FLAGS})

# Headers files definitions
include_directories(${CMAKE_SOURCE_PATH}/externals)
include_directories(${CMAKE_SOURCE_PATH}/clgl/include)
include_directories(${CMAKE_SOURCE_PATH}/image/include)
include_directories(${CMAKE_SOURCE_PATH}/utils/include)
include_directories(${CMAKE_SOURCE_PATH}/${LIB_NAME}/include)

# Adds the library name
add_library(${LIB_NAME} ${SRC})


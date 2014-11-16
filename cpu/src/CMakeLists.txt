# Minimum cmake version
cmake_minimum_required(VERSION 2.6)

# Libary name
set(LIB_NAME cpu)

# Internal files for this module
set(C_FLAGS -std=c++11)
set(SRC image.cpp)

# Compiler flags
add_definitions(${C_FLAGS})

# Headers files definitions
include_directories(${CMAKE_SOURCE_PATH}/${LIB_NAME}/include)

# Adds the library name
add_library(${LIB_NAME} ${SRC})


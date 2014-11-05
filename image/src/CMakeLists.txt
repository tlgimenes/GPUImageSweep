# Minimum cmake version
cmake_minimum_required(VERSION 2.6)

# Libary name
set(LIB_NAME image)

# Internal files for this module
set(SRC image_acquirer.cpp)
set(C_FLAGS -std=c++11)

# Compiler flags
add_definitions(${C_FLAGS})

# Headers files definitions
include_directories(${CMAKE_SOURCE_PATH}/${LIB_NAME}/include)

# Adds the library name
add_library(${LIB_NAME} ${SRC})


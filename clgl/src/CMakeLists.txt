# Minimum cmake version
cmake_minimum_required(VERSION 2.6)

# Libary name
set(LIB_NAME clgl)

# Internal files for this module
set(C_FLAGS -std=c++11)
if(WIN32)
    set(SRC clgl.cpp clgl_single_platform.cpp clgl_single_platform_windows.cpp clgl_error.cpp clgl_windows.cpp)
else(WIN32)
    set(SRC clgl.cpp clgl_single_platform.cpp clgl_single_platform_linux.cpp clgl_error.cpp clgl_linux.cpp)
endif(WIN32)

# Compiler flags
add_definitions(${C_FLAGS})

# Headers files definitions
include_directories(${CMAKE_SOURCE_PATH}/${LIB_NAME}/include
                    ${CMAKE_SOURCE_PATH}/windowManager/include)

# Adds the library name
add_library(${LIB_NAME} ${SRC})


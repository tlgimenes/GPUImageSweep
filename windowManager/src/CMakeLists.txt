# Minimum cmake version
cmake_minimum_required(VERSION 2.6)

# Libary name
set(LIB_NAME windowManager)

# Internal files for this module
set(SRC window_manager.cpp window_manager_glut.cpp main_window_glut.cpp)
set(C_FLAGS -std=c++11)

# Compiler flags
add_definitions(${C_FLAGS})

# Headers files definitions
include_directories(${CMAKE_SOURCE_PATH}/${LIB_NAME}/include)
include_directories(${CMAKE_SOURCE_PATH}/image/include)

# Required external packages
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
find_package(GLUT   REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
find_package(GLEW   REQUIRED)


# Adds the library name
add_library(${LIB_NAME} ${SRC})

target_link_libraries(${LIB_NAME} 
                      ${OPENGL_LIBRARIES} 
                      ${GLUT_LIBRARY}
                      ${GLEW_LIBRARY} 
                      image)

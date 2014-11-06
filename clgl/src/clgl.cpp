/*
 * =====================================================================================
 *       Filename:  clgl.cpp
 *    Description:  
 *        Created:  2014-10-23 16:35
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <string>
#include <map>

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Gets the platforms and devices associated 
 * */
CLGL::CLGL(WindowManager& manager) :
    _platforms(new std::vector<CLGLSinglePlatform>()),
    _map_ker_plat_id(new std::map<int,int>()),
    _dev(CL_DEVICE_TYPE_DEFAULT)
{
    manager.init();
}

CLGL::CLGL(const CLGL& same) :
    _platforms(same.platforms()),
    _map_ker_plat_id(same.map_ker_plat_id())
{
    /* Nothing to do here */
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Get the info from device dev
 */
std::string CLGL::clgl_get_devices_info(cl_device_info name, int platform_id, int device_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).clgl_get_devices_info(name, device_id);
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Build the Program Source
 */
int CLGL::clgl_build_program_source(const std::string& programName, 
        const std::string& compilerFlags,
        int platform_id, int device_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).clgl_build_program_source(programName, 
            compilerFlags, device_id);
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Put the kernel in the last position in the kernel array
 */
int CLGL::clgl_build_kernel(const std::string& kernelFunctionName,
        int platform_id, int device_id)
{
    assert(platform_id < this->_platforms->size());

    int kernel_id = this->_platforms->at(platform_id).clgl_build_kernel(kernelFunctionName,
            device_id);

    this->_map_ker_plat_id->insert(std::pair<int,int>(kernel_id,device_id));

    return kernel_id;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Load the data in hostMemory to OpenCL Device
 */
int CLGL::clgl_load_data_to_device(const cl_bool blocking, const size_t bufferBytesSize,
        const void * hostMemory, const cl_mem_flags flag,
        int platform_id, int device_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).clgl_load_data_to_device(blocking, 
            bufferBytesSize, hostMemory, flag, device_id);
}

////////////////////////////////////////////////////////////////////////////////////////

void CLGL::clgl_load_vbo_data_to_device(const int buffer_id, const size_t bufferBytesSize, 
                const void* hostMemory, int platform_id, int device_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).clgl_load_vbo_data_to_device(buffer_id, 
            bufferBytesSize, hostMemory, device_id);
}


////////////////////////////////////////////////////////////////////////////////////////

int CLGL::clgl_load_texture_data_to_device(const size_t width, const size_t height, 
                const uchar* hostMemory, const cl_mem_flags flag, int platform_id, int device_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).clgl_load_texture_data_to_device(width, height,
            hostMemory, flag, device_id);
}

////////////////////////////////////////////////////////////////////////////////////////

void CLGL::clgl_load_texture_data_to_device(const int texture_id, const size_t width,
                const size_t height, const uchar* hostMemory, 
                int platform_id, int device_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).clgl_load_texture_data_to_device(texture_id, 
            width, height, hostMemory, device_id);
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Get data from device
 */
void CLGL::clgl_get_data_from_device(const int buffer_id, const cl_bool blocking, 
        const int bytes_size, void * data, int platform_id, int device_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).clgl_get_data_from_device(buffer_id, blocking,
            bytes_size, data, device_id);
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Set Arguments to kernel
 */
void CLGL::clgl_set_arg(const int argNum, const int buffer_id, const int kernel_id)
{
    try {
        int platform_id = this->_map_ker_plat_id->at(kernel_id);
        assert(platform_id < this->_platforms->size());

        this->_platforms->at(platform_id).clgl_set_arg(argNum, buffer_id, kernel_id);
    }
    catch(const std::out_of_range& error)
    {
        std::cout << error.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Set Arguments to kernel
 */
void CLGL::clgl_set_arg(const int argNum, const size_t bytesSize, void* buffer, 
        const int kernel_id)
{
    try {
        int platform_id = this->_map_ker_plat_id->at(kernel_id);
        assert(platform_id < this->_platforms->size());

        this->_platforms->at(platform_id).clgl_set_arg(argNum, bytesSize, buffer, kernel_id);
    }
    catch(const std::out_of_range& error)
    {
        std::cout << error.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Run the kernel
 */
void CLGL::clgl_run_kernel(const int kernel_id, const int numThreads)
{
    try {
        int platform_id = this->_map_ker_plat_id->at(kernel_id);

        assert(platform_id < this->_platforms->size());

        this->_platforms->at(platform_id).clgl_run_kernel(kernel_id, numThreads);
    }
    catch(const std::out_of_range& error)
    {
        std::cout << error.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////////////

const CLGL& CLGL::operator = (const CLGL& other)
{
    this->_platforms = other._platforms;
    this->_dev = other._dev;

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////

const std::shared_ptr<std::vector<GLuint>>& CLGL::vbos(int platform_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).vbos();
}

////////////////////////////////////////////////////////////////////////////////////////

const std::shared_ptr<std::vector<GLuint>>& CLGL::textures(int platform_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).textures();
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGL::clgl_get_vbo_bytes_size(int vbo_id, int platform_id)
{
    return this->_platforms->at(platform_id).clgl_get_vbo_bytes_size(vbo_id);
}

////////////////////////////////////////////////////////////////////////////////////////

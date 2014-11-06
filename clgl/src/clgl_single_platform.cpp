/*
 * =====================================================================================
 *       Filename:  clgl_single_platform.cpp
 *    Description:  Class for handling OpenCL OpenGL interoperability with one platform
 *        Created:  2014-10-25 01:16
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_single_platform.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Gets the platforms and devices associated 
 * */
CLGLSinglePlatform::CLGLSinglePlatform(cl::Platform& platform) :
    _vbos(new std::vector<GLuint>()),
    _textures(new std::vector<GLuint>()),
    _platform(&platform),
    _devices(new std::vector<cl::Device>()),
    _contexts(new std::vector<cl::Context>()),
    _commandQueues(new std::vector<cl::CommandQueue>()),
    _programs(new std::vector<cl::Program>()),
    _dev(CL_DEVICE_TYPE_DEFAULT),
    _bufferGL(new std::vector<cl::Memory>()),
    _buffer(new std::vector<cl::Buffer>()),
    _kernel(new std::vector<cl::Kernel>()),
    _map_ker_prog_id(new std::map<int,int>())
{
    try {
        // Get the Devices for each platform//
        _platform->getDevices(_dev, &*_devices);
    }
    catch(const cl::Error& error) 
    {
        clgl_assert(error.err());
    }
}

/**
 *  Constructor for using the shared pointer facility 
 * */
CLGLSinglePlatform::CLGLSinglePlatform(const CLGLSinglePlatform& same) :
    _vbos(same._vbos),
    _textures(same._textures),
    _platform(same._platform),
    _devices(same._devices),
    _contexts(same._contexts),
    _commandQueues(same._commandQueues),
    _programs(same._programs),
    _dev(CL_DEVICE_TYPE_DEFAULT),
    _bufferGL(same._bufferGL),
    _buffer(same._buffer),
    _kernel(same._kernel),
    _map_ker_prog_id(same._map_ker_prog_id)   
{
    /* Nothing to do here ! */
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Get the info from device dev
 */
std::string CLGLSinglePlatform::clgl_get_devices_info(cl_device_info name, int device_id)
{
    std::string info;

    try {
        (*this->_devices)[device_id].getInfo(name, &info);
    }
    catch(const cl::Error& error)
    {
        clgl_assert(error.err());
    }

    return info;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Build the Program Source
 */
int CLGLSinglePlatform::clgl_build_program_source(const std::string& programName, 
        const std::string& compilerFlags, int device_id)
{
    std::fstream programFile; 
    std::stringstream programStr;

    try {
        programFile.open(programName);
        programStr << programFile;
 
        // Builds the Program Source
        cl::Program::Sources source(1, std::make_pair(programStr.str().c_str(),
                    programStr.str().size()));
        this->_programs->push_back(cl::Program((*this->_contexts)[device_id], source));
        this->_programs->back().build((*this->_devices), compilerFlags.c_str());
    }
    catch(const cl::Error& error )
    {
        if(!strcmp(error.what(), "clBuildProgram")){ // If Compilation Errors print then
            std::string log;
            (*this->_programs)[device_id].getBuildInfo((*this->_devices)[device_id], 
                    CL_PROGRAM_BUILD_LOG, &log);
            std::cout << log << std::endl;
        }
        clgl_assert(error.err());
    }
    catch(const std::ios_base::failure& error)
    {
        std::cout << error.what() << std::endl;
    }

    return this->_programs->size()-1;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Put the kernel in the last position in the kernel array
 */
int CLGLSinglePlatform::clgl_build_kernel(
        const std::string& kernelFunctionName, int program_id)
{
    int kernel_id = CLGL_INVALID_KERNEL_ID;

    try{
        this->_kernel->push_back(cl::Kernel((*this->_programs)[program_id], 
                    kernelFunctionName.data()));
        kernel_id = this->_kernel->size() - 1;
        this->_map_ker_prog_id->insert(std::pair<int,int>(kernel_id, program_id));
    }
    catch(const cl::Error& error){
        clgl_assert(error.err());
    }

    return kernel_id;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Load the data in hostMemory to OpenCL Device
 */
int CLGLSinglePlatform::clgl_load_data_to_device(const cl_bool blocking, 
        const size_t bufferBytesSize, const void * hostMemory, const cl_mem_flags flag,
        int device_id)
{
    cl::Buffer buff;
    int buffer_id = CLGL_INVALID_BUFFER_ID;

    try{
        this->_commandQueues->at(device_id).finish();
        buff = cl::Buffer((*this->_contexts)[device_id], flag, bufferBytesSize);

        this->_buffer->push_back(buff);

        this->_commandQueues->at(device_id).enqueueWriteBuffer(buff, blocking, 
                0, bufferBytesSize, hostMemory);

        buffer_id = _buffer->size() - 1;
    }
    catch(const cl::Error& error){
        clgl_assert(error.err());
    }

    return buffer_id;  
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Load vbo data to device
 * */
void CLGLSinglePlatform::clgl_load_vbo_data_to_device(const int buffer_id, 
        const size_t bufferBytesSize, const void* hostMemory, int device_id)
{
    // Wait untill OpenGL finish it's tasks
    glFinish();
    // Wait until OpenCL finish it's tasks
    this->_commandQueues->at(device_id).finish();

    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);                      // activate vbo id to use
    glBufferData(GL_ARRAY_BUFFER, bufferBytesSize, hostMemory, GL_DYNAMIC_DRAW); // load data to Device
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Wait untill OpenGL finish it's tasks
    glFinish();
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Load host data hostMemory to Device in VBO
 */
int CLGLSinglePlatform::clgl_load_texture_data_to_device(const size_t width, const size_t height,
        const uchar* hostMemory, const cl_mem_flags flag, int device_id)
{ 
    GLuint id = 0;  // 0 is reserved, glGenBuffersARB() will return non-zero id if success
    int texture_id = CLGL_INVALID_TEXTURE_ID;

    try {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        // regular sampler params
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //need to set GL_NEAREST (not GL_NEAREST_MIPMAP_* which would cause 
        //CL_INVALID_GL_OBJECT later)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //specify texture dimensions, format etc
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, hostMemory);

        glBindTexture(GL_TEXTURE_2D, 0);
        glFinish();

        // Create buffer from OpenGL Texture
        this->_bufferGL->push_back(cl::BufferGL((*this->_contexts)[device_id], flag, id));

        texture_id = this->_bufferGL->size();
        this->_textures->push_back(texture_id);
    }
    catch(const cl::Error& error)  
    {
        clgl_assert(error.err());
    }

    return texture_id;
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Load data to a previous created texture 
 * */
void CLGLSinglePlatform::clgl_load_texture_data_to_device(const int texture_id, const size_t width,
                const size_t height, const uchar* hostMemory, int device_id)
{
    // Wait until OpenGL finish it's tasks
    glFinish();
    // Wait until OpenCL finish it's tasks
    this->_commandQueues->at(device_id).finish();

    // Write data to the texture 
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, 
                GL_UNSIGNED_BYTE, hostMemory);

    glBindTexture(GL_TEXTURE_2D, 0);
    glFinish();
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Get data from device
 */
void CLGLSinglePlatform::clgl_get_data_from_device(const int buffer_id, const cl_bool blocking,
        const int bytes_size, void * data, int device_id)
{
    // Wait until OpenGL finish it's tasks
    glFinish();
    // Wait until OpenCL finish it's tasks
    this->_commandQueues->at(device_id).finish();
    // Read the data from the buffer 
    this->_commandQueues->at(device_id).enqueueReadBuffer(_buffer->at(buffer_id), 
            blocking, 0, bytes_size, data);

    return;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Set Arguments to kernel
 */
void CLGLSinglePlatform::clgl_set_arg(const int argNum, const int buffer_id, const int kernel_id)
{
    _kernel->at(kernel_id).setArg(argNum, _bufferGL->at(buffer_id));

    return;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Set Arguments to kernel
 */
void CLGLSinglePlatform::clgl_set_arg(const int argNum, const size_t bytesSize, void* buffer, 
        const int kernel_id)
{
    _kernel->at(kernel_id).setArg(argNum, bytesSize, buffer);

    return;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Run the kernel
 */
void CLGLSinglePlatform::clgl_run_kernel(const int kernel_id, 
        const int device_id, const int numThreads)
{
    try{
        // Wait for OpenGL finish what it is doing
        glFinish();

        // Get the GL Objects
        this->_commandQueues->at(device_id).enqueueAcquireGLObjects(
                this->_bufferGL.get(), NULL, NULL);

        //Wait for OpenCL to finish
        this->_commandQueues->at(device_id).finish();

        // Enqueue the kernel
        this->_commandQueues->at(device_id).enqueueNDRangeKernel(_kernel->at(kernel_id),
                cl::NullRange, cl::NDRange(numThreads), cl::NullRange);

        // Wait for OpenCL to finish
        this->_commandQueues->at(device_id).finish();

        // Release the OpenGL objects
        this->_commandQueues->at(device_id).enqueueReleaseGLObjects(
                this->_bufferGL.get(), NULL, NULL);

        //Wait for OpenCL to finish
        this->_commandQueues->at(device_id).finish();
    }
    catch(const cl::Error& error) {
        clgl_assert(error.err());
    }
    catch(const std::out_of_range& error) {
        std::cout << error.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////////////

const std::shared_ptr<std::vector<cl::Device>>& CLGLSinglePlatform::devices()
{
    return this->_devices;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Get OpenGL VBO vector
 */
const std::shared_ptr<std::vector<cl::Memory>>& CLGLSinglePlatform::bufferGL(void)
{
    return this->_bufferGL;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Get OpenCL buffer vector
 */
const std::shared_ptr<std::vector<cl::Buffer>>& CLGLSinglePlatform::buffer(void)
{
    return this->_buffer;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Get Kernels
 */
const std::shared_ptr<std::vector<cl::Kernel>>& CLGLSinglePlatform::kernel(void)
{
    return this->_kernel;
}

////////////////////////////////////////////////////////////////////////////////////////

/** 
 * Get vbos ids
 *  */
const std::shared_ptr<std::vector<GLuint>>& CLGLSinglePlatform::vbos(void)
{
    return this->_vbos;
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Get textures ids 
 * */
const std::shared_ptr<std::vector<GLuint>>& CLGLSinglePlatform::textures(void)
{
    return this->_textures;
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLSinglePlatform::clgl_get_vbo_bytes_size(int vbo_id)
{
    int bufferSize = 0;

    glFinish();

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);                      // activate vbo id to use
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glFinish();

    return bufferSize;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Macro for print devices info
 */
#define PRINTDEVICEINFO(INFO_NAME, STR_INFO_NAME, INFO_TYPE, SPACE) \
    str << "---------------------------------------------------------------" << std::endl; \
    str << STR_INFO_NAME << std::endl; \
    try{ \
        for(unsigned int i=0; i < this->_devices->size(); i++){ \
            (*this->_devices)[i].getInfo(INFO_NAME, &INFO_TYPE); \
            str << "Device " << i << ": "; \
            for(unsigned int j=0; j < INFO_TYPE.size(); j++){ \
                str << INFO_TYPE[j] << SPACE; \
            } \
            str << std::endl; \
        } \
    } \
    catch(cl::Error error){ \
        clgl_assert(error.err()); \
    } \

/*
 * Print All information about the current platform 
 * and all Devices
 */
std::string CLGLSinglePlatform::to_string()
{
    std::string info;
    std::vector<size_t> v;
    std::vector<cl_int> c;
    std::stringstream str;

    str << std::endl << "---------------------------------------------------------------" << std::endl;
    str << "DEVICES INFO  " << std::endl;
    str << "---------------------------------------------------------------" << std::endl << std::endl;

    PRINTDEVICEINFO(CL_DEVICE_NAME, "--->Devices Names:", info, "")
    PRINTDEVICEINFO(CL_DEVICE_VENDOR, "--->Devices Vendors:", info, "")
    PRINTDEVICEINFO(CL_DEVICE_PROFILE, "--->Devices Profiles:", info, "")
    PRINTDEVICEINFO(CL_DEVICE_VERSION, "--->Devices Versions:", info, "")
    PRINTDEVICEINFO(CL_DRIVER_VERSION, "--->Devices Versions:", info, "")
    PRINTDEVICEINFO(CL_DEVICE_OPENCL_C_VERSION, "--->Devices OpenCL C Versions", info, "")
    PRINTDEVICEINFO(CL_DEVICE_EXTENSIONS, "--->Devices Extensions", info, "")
    PRINTDEVICEINFO(CL_DEVICE_MAX_WORK_ITEM_SIZES, "--->Devices Max Work Itens Sizes", v, " ")
    PRINTDEVICEINFO(CL_DEVICE_MAX_WORK_GROUP_SIZE, "--->Devices Max Work Group Sizes", v, " ")
    PRINTDEVICEINFO(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, "--->Devices Max Work Item Dimensions", c, " ")
    PRINTDEVICEINFO(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, "--->Devices Global Mem Cache Size", v, " ")
    PRINTDEVICEINFO(CL_DEVICE_GLOBAL_MEM_SIZE, "--->Devices Global Mem Size", v, " ")
    PRINTDEVICEINFO(CL_DEVICE_MAX_CLOCK_FREQUENCY, "--->Devices Max Clock Frequency", c, " ")
    PRINTDEVICEINFO(CL_DEVICE_MAX_COMPUTE_UNITS, "--->Devices Max Compute Units", c, " ")
    PRINTDEVICEINFO(CL_DEVICE_MAX_MEM_ALLOC_SIZE, "--->Devices Max Mem Alloc Size", v, " ")
    PRINTDEVICEINFO(CL_DEVICE_ADDRESS_BITS, "--->Devices Address Bits", c, " ")
    str << "---------------------------------------------------------------" << std::endl;

    str << std::endl;

    return str.str();
}

////////////////////////////////////////////////////////////////////////////////////////


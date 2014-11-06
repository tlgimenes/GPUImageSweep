/*
 * =====================================================================================
 *       Filename:  clgl_single_platform.hpp
 *    Description:  Class for handling OpenCL OpenGL interoperability with one platform
 *        Created:  2014-10-25 01:03
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef CLGL_SINGLE_PLATFORM_HPP
#define CLGL_SINGLE_PLATFORM_HPP

////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

#define CLGL_INVALID_KERNEL_ID -1
#define CLGL_INVALID_BUFFER_ID -1
#define CLGL_INVALID_VBO_BUFFER_ID -1
#define CLGL_INVALID_TEXTURE_ID -1

////////////////////////////////////////////////////////////////////////////////////////

typedef unsigned char uchar;

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_error.hpp"
#include "window_manager.hpp"

#include <GL/glew.h>
#include <GL/glut.h>
#include <CL/cl.hpp>

#include <iostream>
#include <memory>
#include <map>

////////////////////////////////////////////////////////////////////////////////////////

class CLGLSinglePlatform
{
    protected:
        // -------------------- //
        // GL related variables //
        // -------------------- // 
        std::shared_ptr<std::vector<GLuint>> _vbos;
        std::shared_ptr<std::vector<GLuint>> _textures;

        // -------------------- //
        // CL related variables //
        // -------------------- //
        std::shared_ptr<cl::Platform> _platform;
        std::shared_ptr<std::vector<cl::Device>> _devices;
        std::shared_ptr<std::vector<cl::Context>> _contexts;
        std::shared_ptr<std::vector<cl::CommandQueue>> _commandQueues;
        std::shared_ptr<std::vector<cl::Program>> _programs;
        cl_device_type _dev;

        // All OpenGL VBO buffers are stored here
        std::shared_ptr<std::vector<cl::Memory>> _bufferGL;

        // All OpenCL VBO buffers are stored here
        std::shared_ptr<std::vector<cl::Buffer>> _buffer;

        // All kernels are stored here
        std::shared_ptr<std::vector<cl::Kernel>> _kernel;

        // Maps the kernel_id to each device_id;
        std::shared_ptr<std::map<int,int>> _map_ker_prog_id;

        // ----------------- //
        // Protected Methods //
        // ----------------- //
        //Create Context
        virtual void clgl_create_context(){};
        //Create the command queue
        virtual void clgl_create_command_queue(){};

    public: 
        explicit CLGLSinglePlatform(cl::Platform& platform);
        explicit CLGLSinglePlatform(const CLGLSinglePlatform& same);

        //Get all the information from device
        std::string clgl_get_devices_info(cl_device_info name, int device_id = 0);

        //Build the source of the kernel
        int clgl_build_program_source(const std::string& programName, 
                const std::string& compilerFlags = " ", int device_id = 0);

        //Build one function in the kernel source code
        int clgl_build_kernel(
                const std::string& kernelFunctionName, int device_id = 0);

        //Transfer between host and device
        int clgl_load_data_to_device(const cl_bool blocking,
                const size_t bufferBytesSize, const void * hostMemory, 
                const cl_mem_flags flag, int device_id = 0);
        template <GLenum target>
        int clgl_load_vbo_data_to_device(const size_t bufferBytesSize, const void * hostMemory, 
                const cl_mem_flags flag = CL_MEM_READ_WRITE, int device_id = 0);
        void clgl_load_vbo_data_to_device(const int buffer_id, const size_t bufferBytesSize, 
                const void* hostMemory, int device_id = 0);
        int clgl_load_texture_data_to_device(const size_t width, const size_t height, 
                const uchar* hostMemory, const cl_mem_flags flag = CL_MEM_READ_WRITE, 
                int device_id = 0);
        void clgl_load_texture_data_to_device(const int texture_id, const size_t width,
                const size_t height, const uchar* hostMemory, int device_id = 0);
        void clgl_get_data_from_device(const int buffer_id, const cl_bool blocking, 
                const int bytes_size, void * data, int device_id = 0);


        //Set the kernel arguments for data on device
        void clgl_set_arg(const int argNum, const int buffer_id = 0, const int kernel_id = 0);
        //Set the simple kernel arguments
        void clgl_set_arg(const int argNum, const size_t bytesSize, void* buffer,
                const int kernel_id);

        //Runs the kernel
        void clgl_run_kernel(const int kernel_id = 0, const int device_id = 1, const int numThreads = 1);

        //Get Private stuff
        const std::shared_ptr<std::vector<cl::Device>>& devices();
        const std::shared_ptr<std::vector<cl::Memory>>& bufferGL();
        const std::shared_ptr<std::vector<cl::Buffer>>& buffer();
        const std::shared_ptr<std::vector<cl::Kernel>>& kernel();
        const std::shared_ptr<std::vector<GLuint>>& vbos();
        const std::shared_ptr<std::vector<GLuint>>& textures();

        int clgl_get_vbo_bytes_size(int vbo_id);

        std::string to_string(void);
};

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Load host data hostMemory to Device in VBO
 */
template <GLenum target>
int CLGLSinglePlatform::clgl_load_vbo_data_to_device(const size_t bufferBytesSize,
        const void * hostMemory, const cl_mem_flags flag, int device_id)
{
    GLuint id = 0;  // 0 is reserved, glGenBuffersARB() will return non-zero id if success
    int vbo_buffer_id = CLGL_INVALID_VBO_BUFFER_ID;

    try {
        glGenBuffers(1, &id);                          // create a vbo
        glBindBuffer(target, id);                      // activate vbo id to use
        glBufferData(target, bufferBytesSize, hostMemory, GL_DYNAMIC_DRAW); // create buffer

        // check data size in VBO is same as input array, if not return 0 and delete VBO
        int bufferSize = 0;
        glGetBufferParameteriv(target, GL_BUFFER_SIZE, &bufferSize);
        if((int)bufferBytesSize != bufferSize){
            glDeleteBuffers(1, &id);
            id = 0;
            std::cout << "[createVBO()] Data size is mismatch with input array\n";
            exit(EXIT_FAILURE);
        }

        //this was important for working inside blender!
        glBindBuffer(target, 0);
        glFinish();  // Wait for OpenGL to finish

        // Create buffer from OpenGL VBO
        this->_bufferGL->push_back(cl::BufferGL((*this->_contexts)[device_id], flag, id));

        vbo_buffer_id = _bufferGL->size();
        this->_vbos->push_back(vbo_buffer_id);
    }
    catch(const cl::Error& error)
    {
        clgl_assert(error.err());
    }

    return vbo_buffer_id;
}

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !CLGL_SINGLE_PLATFORM_HPP */

////////////////////////////////////////////////////////////////////////////////////////


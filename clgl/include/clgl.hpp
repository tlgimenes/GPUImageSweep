/*
 * =====================================================================================
 *       Filename:  clgl.hpp
 *    Description:  
 *        Created:  2014-10-23 16:36
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef CLGL_HPP
#define CLGL_HPP

////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_error.hpp"
#include "window_manager.hpp"
#include "clgl_single_platform.hpp"

#include <GL/glew.h>
#include <GL/glut.h>
#include <CL/cl.hpp>

#include <cassert>
#include <memory>

////////////////////////////////////////////////////////////////////////////////////////

class CLGL 
{
    protected:
        std::shared_ptr<std::vector<CLGLSinglePlatform>> _platforms;

        std::shared_ptr<std::map<int,int>> _map_ker_plat_id;

        cl_device_type _dev;

    public: 
        explicit CLGL();
        explicit CLGL(WindowManager& manager);
        explicit CLGL(const CLGL& same);

        //Get all the information from device
        std::string clgl_get_devices_info(cl_device_info name, 
                int platform_id = 0, int device_id = 0);

        //Build the source of the kernel
        int clgl_build_program_source(const std::string& programName, 
                const std::string& compilerFlags = " ",
                int platform_id = 0, int device_id = 0 );

        //Build one function in the kernel source code
        int clgl_build_kernel(const std::string& kernelFunctionName, 
                int platform_id = 0, int device_id = 0);

        //Transfer between host and device
        int clgl_load_data_to_device(const cl_bool blocking, const size_t bufferBytesSize, 
                const void * hostMemory, const cl_mem_flags flag,
                int platform_id = 0, int device_id = 0);
        template <GLenum target>
        int clgl_load_vbo_data_to_device(const size_t bufferBytesSize, 
                const void * hostMemory, const cl_mem_flags flag,
                int platform_id = 0, int device_id = 0);
        void clgl_load_vbo_data_to_device(const int buffer_id, const size_t bufferBytesSize, 
                const void* hostMemory, int platform_id = 0, int device_id = 0);
        int clgl_load_texture_data_to_device(const size_t width, const size_t height, 
                const uchar* hostMemory, const cl_mem_flags flag = CL_MEM_READ_WRITE, 
                int platform_id = 0, int device_id = 0);
        void clgl_load_texture_data_to_device(const int texture_id, const size_t width,
                const size_t height, const uchar* hostMemory, 
                int platform_id = 0, int device_id = 0);
        void clgl_get_data_from_device(const int buffer_id, const cl_bool blocking, 
                const int bytes_size, void * data,
                int platform_id = 0, int device_id = 0);

        //Set the kernel arguments for data on device
        void clgl_set_arg(const int argNum, const int buffer_id, const int kernel_id);
        //Set the simple kernel arguments
        void clgl_set_arg(const int argNum, const size_t bytesSize, void* buffer, const int kernel_id);

        //Runs the Kernel
        void clgl_run_kernel(const int kernel_id, const int numThreads);

        const CLGL& operator = (const CLGL& other);

        inline const std::shared_ptr<std::vector<CLGLSinglePlatform>>& platforms() const {return this->_platforms;}

        inline const std::shared_ptr<std::map<int,int>>& map_ker_plat_id() const {return this->_map_ker_plat_id;}

        const std::shared_ptr<std::vector<GLuint>>& vbos(int platform_id);
        const std::shared_ptr<std::vector<GLuint>>& textures(int platform_id);

        int clgl_get_vbo_bytes_size(int vbo_id, int platform_id);
};

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Load host data hostMemory to Device in VBO
 */
template <GLenum target>
int CLGL::clgl_load_vbo_data_to_device(const size_t bufferBytesSize, 
        const void * hostMemory, const cl_mem_flags flag, int platform_id, int device_id)
{
    assert(platform_id < this->_platforms->size());

    return this->_platforms->at(platform_id).clgl_load_vbo_data_to_device<target>(bufferBytesSize,
            hostMemory, flag, device_id);
}

////////////////////////////////////////////////////////////////////////////////////////

inline std::ostream& operator << (std::ostream& o, const CLGL& b)
{
    for(int i=0; i < b.platforms()->size(); i++) 
    {
        o << b.platforms()->at(i).to_string();
    }
    return o;
}

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !CLGL_HPP */

////////////////////////////////////////////////////////////////////////////////////////


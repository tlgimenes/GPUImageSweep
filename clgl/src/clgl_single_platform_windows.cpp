/*
 * =====================================================================================
 *       Filename:  clgl_windows.cpp
 *    Description:  
 *        Created:  2014-10-23 16:58
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_error.hpp"
#include "clgl_windows.hpp"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <CL/cl.hpp>

#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <string>

#define GL_GLEXT_PROTOTYPES

#include <GL/wgl.h>

////////////////////////////////////////////////////////////////////////////////////////

CLGLSinglePlatformWindows::CLGLSinglePlatformWindows(WindowManager& manager, cl::Platform& plat) :
    CLGLSinglePlatform(manager, plat)
{
    // Init OpenCL OpenGL Shared Context //
    this->clgl_create_context();

    // Init OpenCL Command Queue //
    this->clgl_create_command_queue();
}

CLGLSinglePlatformWindows::CLGLSinglePlatformWindows(const CLGLSinglePlatformWindows& same) :
    CLGLSinglePlatform(same)
{
    /* Nothing to do here */
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Create Context. For each Platform this method must be diferent
 */
void CLGLSinglePlatformWindows::clgl_create_context()
{
    // Creates the Properties Array
    cl_context_properties properties[] = {
        CL_GL_CONTEXT_KHR, (cl_context_properties) wglGetCurrentContext(), 
        CL_WGL_HDC_KHR, (cl_context_properties) wglGetCurrentDC(), 
        CL_CONTEXT_PLATFORM, (cl_context_properties) (*this->_platform)), 
        0
    };

    try{
        // Creates the Context
        for(int i=0; i < this->_devices->size(); i++)
        {
            (*this->_context)[i] = cl::Context(this->_dev, properties);
        }
    }
    catch(cl::Error error){
        clgl_assert(error.err());
    }
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Creates the Command Queue. This Method associates the command
 * queue to the first OpenCL device found, if you want another 
 * device just change the index on the device array below
 */
void CLGLSinglePlatformWindows::clgl_create_command_queue()
{
    try{
        (*this->_commandQueue)[i] = cl::CommandQueue((*this->_context)[i],
                *(this->_device)[i], 0);
    }
    catch(cl::Error error){
        clgl_assert(error.err());
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////////////

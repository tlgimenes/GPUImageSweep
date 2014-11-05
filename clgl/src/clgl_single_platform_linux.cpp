/*
 * =====================================================================================
 *       Filename:  clgl_linux.cpp
 *    Description:  
 *        Created:  2014-10-23 16:56
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_error.hpp"
#include "clgl_single_platform_linux.hpp"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <CL/cl.hpp>

#define GL_GLEXT_PROTOTYPES

#include <GL/glx.h>

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////

CLGLSinglePlatformLinux::CLGLSinglePlatformLinux(WindowManager& manager, cl::Platform& platform) :
    CLGLSinglePlatform(manager, platform)
{
    // Init OpenCL OpenGL Shared Context //
    this->clgl_create_context();

    // Init OpenCL Command Queue //
    this->clgl_create_command_queue();
}

CLGLSinglePlatformLinux::CLGLSinglePlatformLinux(const CLGLSinglePlatformLinux& same)
    : CLGLSinglePlatform(same)
{
    /* Nothing to do here */
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Create Context. For each Platform this method must be diferent
 */
void CLGLSinglePlatformLinux::clgl_create_context()
{
    // Creates the Properties Array
    cl_context_properties properties[] = 
    {
        CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
        CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(), 
        CL_CONTEXT_PLATFORM, (cl_context_properties) (*this->_platform)(), 
        0
    };

    try {
        // Creates the Context
        for(int i=0; i < this->_devices->size(); i++) 
        {
            this->_contexts->push_back(cl::Context(this->_dev, properties));
        }
    }
    catch(cl::Error error) {
        clgl_assert(error.err());
    }
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Creates the Command Queue. This Method associates the command
 * queue to the first OpenCL device found, if you want another 
 * device just change the index on the device array below
 */
void CLGLSinglePlatformLinux::clgl_create_command_queue()
{
    try{
        for(int i=0; i < this->_devices->size(); i++)
        {
            this->_commandQueues->push_back(cl::CommandQueue(_contexts->at(i), _devices->at(i), 0));
        }
    }
    catch(cl::Error error) {
        clgl_assert(error.err());
    }

    return;
}

////////////////////////////////////////////////////////////////////////////////////////

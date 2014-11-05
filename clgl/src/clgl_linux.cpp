/*
 * =====================================================================================
 *       Filename:  clgl_linux.cpp
 *    Description:  
 *        Created:  2014-11-02 16:32
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_linux.hpp"
#include "clgl_single_platform_linux.hpp"

#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////

CLGLLinux::CLGLLinux(WindowManager& manager) : CLGL()
{
    try {
        std::vector<cl::Platform> platforms;

        // Get the Platforms
        cl::Platform::get(&platforms);

        for(int i=0; i < platforms.size(); i++)
        {
            this->_platforms->push_back(CLGLSinglePlatformLinux(manager, platforms[i]));
        }
    }
    catch(const cl::Error& error) 
    {
        clgl_assert(error.err());
    }
}

CLGLLinux::CLGLLinux(const CLGLLinux& same) :
    CLGL(same)
{
    /* Nothing to do here */
}

////////////////////////////////////////////////////////////////////////////////////////


/*
 * =====================================================================================
 *       Filename:  clgl_windows.cpp
 *    Description:  
 *        Created:  2014-11-02 16:34
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_windows.hpp"
#include "clgl_single_platform_windows.hpp"

////////////////////////////////////////////////////////////////////////////////////////

CLGLWindows::CLGLWindows(WindowManager& manager) : CLGL()
{
    try {
        std::vector<cl::Platform> platforms;

        // Get the Platforms
        cl::Platform::get(&platforms);

        for(int i=0; i < platforms.size(); i++)
        {
            this->_platforms->push_back(CLGLSinglePlatformWindows(manager, platforms[i]));
        }
    }
    catch(const cl::Error& error) 
    {
        clgl_assert(error.err());
    }
}

CLGLWindows::CLGLWindows(const CLGLWindows& same) :
    CLGL(same)
{
    /* Nothing to do here */
}

///////////////////////////////////////////////////////////////////////////////////////

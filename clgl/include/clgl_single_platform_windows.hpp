/*
 * =====================================================================================
 *       Filename:  clgl_windows.hpp
 *    Description:  
 *        Created:  2014-10-23 16:55
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef CLGL_SINGLE_PLATFORM_WINDOWS_HPP
#define CLGL_SINGLE_PLATFORM_WINDOWS_HPP

////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_error.hpp"
#include "clgl_single_platform.hpp"

////////////////////////////////////////////////////////////////////////////////////////

class CLGLSinglePlatformWindows: public CLGLSinglePlatform
{
  public:
    CLGLSinglePlatformWindows(cl::Platform* platform);
    CLGLSinglePlatformWindows(const CLGLSinglePlatformWindows& same);

  protected:
    void clgl_create_context();
    void clgl_create_command_queue();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !CLGL_SINGLE_PLATFORM_WINDOWS_HPP */

////////////////////////////////////////////////////////////////////////////////////////


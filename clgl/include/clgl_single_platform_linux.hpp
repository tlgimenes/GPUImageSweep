/*
 * =====================================================================================
 *       Filename:  clgl_linux.hpp
 *    Description:  
 *        Created:  2014-10-23 16:53
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef CLGL_SINGLE_PLATFORM_LINUX_HPP
#define CLGL_SINGLE_PLATFORM_LINUX_HPP

////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CL_ENABLE_EXCEPTIONS
#define __CL_ENABLE_EXCEPTIONS
#endif

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_error.hpp"
#include "clgl_single_platform.hpp"

////////////////////////////////////////////////////////////////////////////////////////

class CLGLSinglePlatformLinux: public CLGLSinglePlatform
{
  public:
    CLGLSinglePlatformLinux(WindowManager& manager, cl::Platform& platform);
    CLGLSinglePlatformLinux(const CLGLSinglePlatformLinux& same);

  protected:
    void clgl_create_context();
    void clgl_create_command_queue();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !CLGL_SINGLE_PLATFORM_LINUX_HPP */

////////////////////////////////////////////////////////////////////////////////////////


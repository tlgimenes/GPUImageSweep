/*
 * =====================================================================================
 *       Filename:  clgl_error.cpp
 *    Description:  
 *        Created:  2014-10-23 16:44
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_error.hpp"

#include <CL/cl.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////

#define CASE(cl_error) \
            case cl_error: \
                return #cl_error

////////////////////////////////////////////////////////////////////////////////////////

void __clgl_assert(GLenum err, const char* file, int line)
{
    if(err != GL_NO_ERROR)
    {
        std::cout << CLGLError::to_str(err) << " in " << file << ":" << line << std::endl;
        exit(EXIT_FAILURE);
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void __clgl_assert(int err, const char* file, int line)
{
    if(err != GL_NO_ERROR)
    {
        std::cout << CLGLError::to_str(err) << " in " << file << ":" << line << std::endl;
        exit(EXIT_FAILURE);
    }
}

////////////////////////////////////////////////////////////////////////////////////////

std::string CLGLError::to_str(GLenum err)
{
    switch(err) 
    {
        // No error has been recorded. The value of this symbolic constant 
        // is guaranteed to be 0.
        CASE(GL_NO_ERROR);

        //An unacceptable value is specified for an enumerated argument.
        // The offending command is ignored and has no other side effect than to set
        // the error flag.
        CASE(GL_INVALID_ENUM);

        //A numeric argument is out of range. The offending command is ignored and 
        //has no other side effect than to set the error flag.
        CASE(GL_INVALID_VALUE);

        //The specified operation is not allowed in the current state. The offending 
        //command is ignored and has no other side effect than to set the error flag.
        CASE(GL_INVALID_OPERATION);

        //The framebuffer object is not complete. The offending command is ignored and 
        //has no other side effect than to set the error flag.
        CASE(GL_INVALID_FRAMEBUFFER_OPERATION);

        // There is not enough memory left to execute the command. The state of the GL 
        // is undefined, except for the state of the error flags, after this error is recorded.
        CASE(GL_OUT_OF_MEMORY);

        //An attempt has been made to perform an operation that would cause an internal 
        //stack to underflow.
        CASE(GL_STACK_UNDERFLOW);

        CASE(GL_STACK_OVERFLOW);

    }
}

////////////////////////////////////////////////////////////////////////////////////////

std::string CLGLError::to_str(int err)
{
  switch(err){
    CASE(CL_SUCCESS);
    CASE(CL_DEVICE_NOT_FOUND);
    CASE(CL_DEVICE_NOT_AVAILABLE);
    CASE(CL_COMPILER_NOT_AVAILABLE);
    CASE(CL_MEM_OBJECT_ALLOCATION_FAILURE);
    CASE(CL_OUT_OF_RESOURCES);
    CASE(CL_OUT_OF_HOST_MEMORY);
    CASE(CL_PROFILING_INFO_NOT_AVAILABLE);
    CASE(CL_MEM_COPY_OVERLAP);
    CASE(CL_IMAGE_FORMAT_MISMATCH);
    CASE(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CASE(CL_BUILD_PROGRAM_FAILURE);
    CASE(CL_MAP_FAILURE);
    CASE(CL_INVALID_VALUE);
    CASE(CL_INVALID_DEVICE_TYPE);
    CASE(CL_INVALID_PLATFORM);
    CASE(CL_INVALID_DEVICE);
    CASE(CL_INVALID_CONTEXT);
    CASE(CL_INVALID_QUEUE_PROPERTIES);
    CASE(CL_INVALID_COMMAND_QUEUE);
    CASE(CL_INVALID_HOST_PTR);
    CASE(CL_INVALID_MEM_OBJECT);
    CASE(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CASE(CL_INVALID_IMAGE_SIZE);
    CASE(CL_INVALID_SAMPLER);
    CASE(CL_INVALID_BINARY);
    CASE(CL_INVALID_BUILD_OPTIONS);
    CASE(CL_INVALID_PROGRAM);
    CASE(CL_INVALID_PROGRAM_EXECUTABLE);
    CASE(CL_INVALID_KERNEL_NAME);
    CASE(CL_INVALID_KERNEL_DEFINITION);
    CASE(CL_INVALID_KERNEL);
    CASE(CL_INVALID_ARG_INDEX);
    CASE(CL_INVALID_ARG_VALUE);
    CASE(CL_INVALID_ARG_SIZE);
    CASE(CL_INVALID_KERNEL_ARGS);
    CASE(CL_INVALID_WORK_DIMENSION);
    CASE(CL_INVALID_WORK_GROUP_SIZE);
    CASE(CL_INVALID_WORK_ITEM_SIZE);
    CASE(CL_INVALID_GLOBAL_OFFSET);
    CASE(CL_INVALID_EVENT_WAIT_LIST);
    CASE(CL_INVALID_EVENT);
    CASE(CL_INVALID_OPERATION);
    CASE(CL_INVALID_GL_OBJECT);
    CASE(CL_INVALID_BUFFER_SIZE);
    CASE(CL_INVALID_MIP_LEVEL);
    CASE(CL_INVALID_GLOBAL_WORK_SIZE);
  }
  return std::string("NO_ERROR_DETECTED ! :("); 
}

////////////////////////////////////////////////////////////////////////////////////////


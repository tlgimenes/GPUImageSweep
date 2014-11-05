/*
 * =====================================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Created:  2014-10-22 17:13 
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl.hpp"
#include "clgl_windows.hpp"
#include "clgl_linux.hpp"
#include "window_manager_glut.hpp"
#include "image_acquirer.hpp"
#include "main_window_glut.hpp"

#include <CL/cl.hpp>
#include <vector>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////
 
/* ======= Function ==================================================
 *   Name: main
 *   Description: main entry Function
 * =================================================================== 
 */
int main(int argc, char *argv[])
{
    ImageAcquirer img;
    WindowManagerGlut wmanager(argc, argv, "GPUImageSweep", img);
#ifdef WIN32
    CLGLWidows clgl = CLGLWidows(wmanager);
#else
    CLGLLinux clgl = CLGLLinux(wmanager);
#endif

    // Prints platforms and devices details
    std::cout << clgl.platforms()->size() << " platform(s) found !" << std::endl;
    for(int i=0; i < clgl.platforms()->size(); i++)
    {
        std::cout << clgl.platforms()->at(i).devices()->size() << " device(s) found for platform " << i << " !" << std::endl;
    }
    std::cout << clgl << std::endl;

    MainWindowGlut::start(img, clgl, wmanager);

    // main loop !
    wmanager.main_loop();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////


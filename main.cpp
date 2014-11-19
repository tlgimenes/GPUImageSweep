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
#include <fstream>

////////////////////////////////////////////////////////////////////////////////////////
 
/* ======= Function ==================================================
 *   Name: main
 *   Description: main entry Function
 * =================================================================== 
 */
int main(int argc, char *argv[])
{
    float d_min = 237;   // Minimal distance
    float d_max = 280;   // Maximal distance
    float n_planes = 40; // Number of planes to use

    std::ifstream f1("../samples/face00.txt"); // File containing the matrix P of image 1
    std::ifstream f2("../samples/face01.txt"); // File containing the matrix P of image 2

    // Acquire images
    ImageAcquirer img("../samples/face00R.tif", "../samples/face01R.tif"); 

    // Creates OpenGL context with GLUT
    WindowManagerGlut wmanager(argc, argv, "GPUImageSweep", img); 

    // Specific platform clgl
#ifdef WIN32
    CLGLWidows clgl = CLGLWidows(wmanager);
#else
    CLGLLinux clgl = CLGLLinux(wmanager);
#endif

    // Prints platforms and devices details
    std::cout << clgl << std::endl;

    // Reads matrices P = (A|b)
    cv::Matx33d A1, A2;
    cv::Vec3d B1, B2;
    for (int i=0;i<3;i++) f1 >> A1(i,0) >> A1(i,1) >> A1(i,2) >> B1[i]; 
    for (int i=0;i<3;i++) f2 >> A2(i,0) >> A2(i,1) >> A2(i,2) >> B2[i]; 

    // Init callbacks and all the program
    MainWindowGlut::start(img, clgl, wmanager, A1, A2, B1, B2, d_min, d_max, n_planes);

    // main loop !
    wmanager.main_loop();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////


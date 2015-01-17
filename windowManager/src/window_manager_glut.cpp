/*
 * =====================================================================================
 *       Filename:  window_manager_glut.cpp
 *    Description:  Class thats implements the interface windowManager for the
 *    glut/glew window manager 
 *        Created:  2014-10-24 10:48
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "window_manager_glut.hpp"
#include "image_acquirer.hpp"

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>

#define GL_GLEXT_PROTOTYPES

#include <GL/glx.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////////////

WindowManagerGlut::WindowManagerGlut(int argc, char* argv[], std::string window_name,
        ImageAcquirer& img) :
    WindowManager()
{
    _window_width  = img.acquirer1().width();
    _window_height = img.acquirer1().height();

    _window_name = window_name;

    _argc = argc;

    _argv = argv;
}

////////////////////////////////////////////////////////////////////////////////////////

void WindowManagerGlut::init()
{
    /* Init glut and glew */
    glutInit(&_argc, _argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    _window_width  = std::min(glutGet(GLUT_SCREEN_WIDTH ),_window_width);
    _window_height = std::min(glutGet(GLUT_SCREEN_HEIGHT),_window_height);
    std::cout << _window_width << "x" << _window_height << std::endl;
    glutInitWindowSize(_window_width, _window_height);

    glutInitWindowPosition (glutGet(GLUT_SCREEN_WIDTH)/2 -_window_width /2, 
            glutGet(GLUT_SCREEN_HEIGHT)/2 - _window_height/2);

    this->_windowID = glutCreateWindow(_window_name.c_str());

    glewInit();
}

////////////////////////////////////////////////////////////////////////////////////////

void WindowManagerGlut::main_loop()
{
    glutMainLoop();
}

////////////////////////////////////////////////////////////////////////////////////////

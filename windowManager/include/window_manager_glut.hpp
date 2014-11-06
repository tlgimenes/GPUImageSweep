/*
 * =====================================================================================
 *       Filename:  window_manager_glut.hpp
 *    Description:  Class thats implements the interface windowManager for the
 *    glut/glew window manager 
 *        Created:  2014-10-24 10:50
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef WINDOW_MANAGER_GLUT_HPP
#define WINDOW_MANAGER_GLUT_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include "window_manager.hpp"
#include "image_acquirer.hpp"

#include <string>

////////////////////////////////////////////////////////////////////////////////////////

class WindowManagerGlut : public WindowManager
{
    private:
        int _window_height, _window_width;

        std::string _window_name;

        int _argc;

        char** _argv;

    public:
        WindowManagerGlut(int argc, char* argv[], std::string window_name, ImageAcquirer& img);
 
        /** 
         * Init Glut
         * */
        void init();

        /**
         * Glut's main loop
         * */
        void main_loop();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !WINDOW_MANAGER_GLUT_HPP */

////////////////////////////////////////////////////////////////////////////////////////

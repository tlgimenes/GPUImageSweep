/*
 * =====================================================================================
 *       Filename:  main_window_glut.hpp
 *    Description:  Static class contanning callbacks and objects
 *        Created:  2014-11-03 18:13
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef MAIN_WINDOW_GLUT_HPP
#define MAIN_WINDOW_GLUT_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include "image_acquirer.hpp"
#include "window_manager.hpp"
#include "clgl.hpp"

////////////////////////////////////////////////////////////////////////////////////////

#define ON  1
#define OFF 0

////////////////////////////////////////////////////////////////////////////////////////

class MainWindowGlut
{
    public:
        static WindowManager* wmanager;
        static ImageAcquirer* img;
        static CLGL* clgl;

        static bool showInfo;
        static int play;

        // fps counter
        static float fps;

        // Font parameters
        static int fontSize;
        static float stringColor[4];
        static void* font;

        // Mouse parameters
        static int mouse_buttons;
        static float translate_z;
        static float rotate_x;
        static float rotate_y;
        static int mouse_old_x;
        static int mouse_old_y;
        static float scale[3];

        static void start(ImageAcquirer& img, CLGL& clgl, WindowManager& wmanager);

        /**
         * Glut's callbacks 
         * */
        static void glutDisplayFunc_cb();
        static void glutTimerFunc_cb(int ms);
        static void glutIdleFunc_cb();
        static void glutKeyboardFunc_cb(unsigned char key, int x, int y);
        static void glutSpecialKeys_cb(int key, int x, int y);
        static void glutMouse_cb(int button, int state, int x, int y);
        static void glutMotion_cb(int x, int y);
 
        static void drawString(const char *str, int x, int y, float color[4], void *font);
        static void drawInfo(void);
        static void calculateFPS(void);
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !MAIN_WINDOW_GLUT_HPP */

////////////////////////////////////////////////////////////////////////////////////////


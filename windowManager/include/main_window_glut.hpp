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
#include "clgl_image.hpp"
#include "window_manager.hpp"
#include "clgl.hpp"
#include "plane_sweep.hpp"

////////////////////////////////////////////////////////////////////////////////////////

#define ON  1
#define OFF 0

////////////////////////////////////////////////////////////////////////////////////////

class MainWindowGlut
{
    public:
        static WindowManager* wmanager; // manager
        static CLGLImage *img1, *img2;  // CL-GL image interface
        static CLGLImage *img;          // current image beeing shown
        static CLGLProjection *proj;    // CL-GL projection matrix interface 
        static PlaneSweep* planeSweep;  // Plane sweep algorithm 
        static CLGL* clgl;              // CLGL manager

        static int height, width; // app height and width

        static bool showInfo;   // show FPS...
        static int play;        // Run the plane_sweep algorithm 

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

        /**
         * Creates CLGL images, CLGL projection matrices and sets the plane
         * sweep algorithm. This fuction also sets some callbacks for GLUT
         * */
        static void start(ImageAcquirer& img, CLGL& clgl, WindowManager& wmanager, 
                cv::Matx33d A1, cv::Matx33d A2, cv::Vec3d B1, cv::Vec3d B2, 
                float d_min, float d_max, int n_planes);

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


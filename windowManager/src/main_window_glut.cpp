/*
 * =====================================================================================
 *       Filename:  main_window_glut.cpp
 *    Description:  Static class containing callbacks and objects
 *        Created:  2014-11-03 18:17
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "main_window_glut.hpp"
#include "image_acquirer_single_camera.hpp"
#include "clgl_projection.hpp"

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

////////////////////////////////////////////////////////////////////////////////////////

WindowManager* MainWindowGlut::wmanager = NULL;
CLGLImage* MainWindowGlut::img1 = NULL;
CLGLImage* MainWindowGlut::img2 = NULL;
CLGLProjection* MainWindowGlut::proj = NULL;
PlaneSweep* MainWindowGlut::planeSweep = NULL;
CLGL* MainWindowGlut::clgl = NULL;

int MainWindowGlut::height = 0;
int MainWindowGlut::width = 0;

// Hide/Show information
bool MainWindowGlut::showInfo = true;

// FPS counter
float MainWindowGlut::fps = 0.0;

// String parametert_length 
int MainWindowGlut::fontSize = 30;
float MainWindowGlut::stringColor[4] = {1.0,  1.0, 1.0, 1.0};
void* MainWindowGlut::font = GLUT_BITMAP_TIMES_ROMAN_24;

// Execute/Stop computation
int MainWindowGlut::play = ON;

// Mouse and orientation stuff
int MainWindowGlut::mouse_buttons = 0;
float  MainWindowGlut::translate_z = -1.f;
float  MainWindowGlut::rotate_x = 0.0;
float  MainWindowGlut::rotate_y = 0.0;
int MainWindowGlut::mouse_old_x = 0;
int MainWindowGlut::mouse_old_y = 0;
float MainWindowGlut::scale[3] = {1.7, 1.7, 0.3};

////////////////////////////////////////////////////////////////////////////////////////

void MainWindowGlut::start(ImageAcquirer& img, CLGL& clgl, WindowManager& wmanager)
{
    /* Sets objects */
    MainWindowGlut::clgl= &clgl;
    MainWindowGlut::wmanager = &wmanager;

    MainWindowGlut::img1 = new CLGLImage(clgl, img.acquirer1());
    MainWindowGlut::img2 = new CLGLImage(clgl, img.acquirer2());

    cv::Matx33d A1, A2;
    cv::Vec3d B1, B2;
    std::ifstream f1("../samples/face00.txt");
    std::ifstream f2("../samples/face01.txt");
    for (int i=0;i<3;i++) f1 >> A1(i,0) >> A1(i,1) >> A1(i,2) >> B1[i]; 
    for (int i=0;i<3;i++) f2 >> A2(i,0) >> A2(i,1) >> A2(i,2) >> B2[i]; 

    MainWindowGlut::proj = new CLGLProjection(A1, B1, A2, B2, 150, 360, 600, *MainWindowGlut::clgl);
    MainWindowGlut::planeSweep = new PlaneSweep(*MainWindowGlut::clgl, *MainWindowGlut::proj, 
            *MainWindowGlut::img1, *MainWindowGlut::img2);

    height = img.acquirer1().height();
    width  = img.acquirer1().width();

    /* Connect callbacks */
    // Idle function callback
    glutDisplayFunc(MainWindowGlut::glutDisplayFunc_cb);
    glutTimerFunc(30, MainWindowGlut::glutTimerFunc_cb, 30);
    glutIdleFunc(MainWindowGlut::glutIdleFunc_cb);
    glutKeyboardFunc(MainWindowGlut::glutKeyboardFunc_cb);
    glutSpecialFunc(MainWindowGlut::glutSpecialKeys_cb);
    glutMouseFunc(MainWindowGlut::glutMouse_cb);
    glutMotionFunc(MainWindowGlut::glutMotion_cb);

    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 1.0);
    glDisable(GL_DEPTH_TEST);

    // viewport
    glViewport(0, 0, width, height);

    // projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLfloat)width / 
            (GLfloat)height, 0.01, 1000.0);

    // set view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, MainWindowGlut::translate_z);
    glScalef(MainWindowGlut::scale[0], MainWindowGlut::scale[1], MainWindowGlut::scale[2]);

    }

////////////////////////////////////////////////////////////////////////////////////////

/* C GLUT callbacks */

////////////////////////////////////////////////////////////////////////////////////////

void MainWindowGlut::glutIdleFunc_cb()
{
    MainWindowGlut::calculateFPS();

//    MainWindowGlut::img1->update();

//    MainWindowGlut::planeSweep->run_compute_plane_kernel();
//    MainWindowGlut::planeSweep->run_project_plane_kernel();
}

////////////////////////////////////////////////////////////////////////////////////////

void MainWindowGlut::glutDisplayFunc_cb()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    // -------------------------- //
    // Draw information on screen //
    // -------------------------- //
    // Draw info on screen as FPS etc
    MainWindowGlut::drawInfo();

    // ------------------------------ //
    // Render the particles from VBOs //
    // ------------------------------ //
    //render the particles from VBOs
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(1.0);

    // Enable Cient State
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);
    clgl_assert(glGetError());

    //Need to disable these for blender
    glDisableClientState(GL_NORMAL_ARRAY);

    // Binds the vertex VBO's
    glBindBuffer(GL_ARRAY_BUFFER, MainWindowGlut::img1->vertex_coord_vbo_id());
    glVertexPointer(3, GL_FLOAT, sizeof(point4D<GLfloat>), 0);
    clgl_assert(glGetError());

    // VBO Color must be inserted in last place
    glBindBuffer(GL_ARRAY_BUFFER, MainWindowGlut::img1->vertex_color_vbo_id());
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(point4D<GLubyte>), 0);
    clgl_assert(glGetError());

    // Indexed rendering for better performance !
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MainWindowGlut::img1->vertex_index_vbo_id());
    clgl_assert(glGetError());

    // Draw the triangles !
    glDrawElements (GL_TRIANGLES, MainWindowGlut::img1->num_index_elements(), GL_UNSIGNED_INT, 0);
    clgl_assert(glGetError());

    glFlush();

    // Disable Client State
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_INDEX_ARRAY);
    clgl_assert(glGetError());

    // Bind the buffers to zero
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    clgl_assert(glGetError());

    glPopMatrix();

    // Swap the buffers
    glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Draw info strings on screen, like FPS
 * */
void MainWindowGlut::drawInfo(void)
{
    if(MainWindowGlut::showInfo == true){
        std::stringstream fps(std::stringstream::in | std::stringstream::out);
        std::stringstream simTime(std::stringstream::in | std::stringstream::out);
        static std::stringstream pause(std::stringstream::in | std::stringstream::out);
        static std::stringstream kernel(std::stringstream::in | std::stringstream::out);

        pause.seekp(std::ios::beg);
        kernel.seekp(std::ios::beg);

        // Draw FPS
        fps << "FPS: " << MainWindowGlut::fps;
        drawString(fps.str().c_str(), 1, MainWindowGlut::height-MainWindowGlut::fontSize, MainWindowGlut::stringColor, MainWindowGlut::font);

        // Draw How many particles are beeing simulated and if it is paused
        if(MainWindowGlut::play == ON){
            pause << "Simulating ";
            drawString(pause.str().c_str(), 1, 33, MainWindowGlut::stringColor, MainWindowGlut::font);
        }
        else{
            drawString("Paused", 1, 33, MainWindowGlut::stringColor, MainWindowGlut::font);
        }

        // Draw Current Kernel in Use
        kernel << "Kernel: ?" << std::endl;
        drawString(kernel.str().c_str(), 1, 7, MainWindowGlut::stringColor, MainWindowGlut::font);
    }
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Write 2d string using GLUT
 * */
void MainWindowGlut::drawString(const char *str, int x, int y, float color[4], void *font)
{
    //backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    //set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);     // switch to projection matrix
    glPushMatrix();                  // save current projection matrix
    glLoadIdentity();                // reset projection matrix
    gluOrtho2D(0, MainWindowGlut::width, 0, MainWindowGlut::height);  // set to orthogonal projection

    glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask

    glColor4fv(color);          // set text color

    glRasterPos2i(x, y);        // place text position

    // loop all characters in the string
    while(*str){
        glutBitmapCharacter(font, *str);
        ++str;
    }

    glPopAttrib();

    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix  
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Calculates the frames per second
 * */
void MainWindowGlut::calculateFPS(void)
{
    static int frameCount = 0;
    static int previousTime = 0;
    int currentTime = 0;

    //  Increase frame count
    frameCount++;

    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);

    //  Calculate time passed
    int timeInterval = currentTime - previousTime;

    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        MainWindowGlut::fps = frameCount / (timeInterval / 1000.0f);

        //  Set time
        previousTime = currentTime;

        //  Reset frame count
        frameCount = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void MainWindowGlut::glutTimerFunc_cb(int ms)
{ 
    //this makes sure the appRender function is called every ms miliseconds
    glutTimerFunc(ms, glutTimerFunc_cb, ms);
    glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Defines what each key does
 */
void MainWindowGlut::glutKeyboardFunc_cb(unsigned char key, int x, int y)
{
    //this way we can exit the program cleanly
    switch(key)
    {
        // ---------- //
        // QUIT CASES //
        // ---------- //
        case '\033': // escape quits
        case '\015': // Enter quits    
        case 'Q':    // Q quits
        case 'q':    // q (or escape) quits
            // Cleanup up and QUIT
            exit(EXIT_SUCCESS);
            break;
        case 'R': case 'r':
            MainWindowGlut::planeSweep->run_compute_plane_kernel();
            MainWindowGlut::planeSweep->run_project_plane_kernel();
            float * data = new float[30*4];
            MainWindowGlut::clgl->clgl_get_data_from_device(
                    MainWindowGlut::proj->buffer_id_homography(), CL_TRUE, 
                sizeof(point4D<GLfloat>)*30, data);

            for(int i=0; i < 30*4; i++)
            {
                std::cout << data[i] << " ";
            } std::cout << std::endl;

            break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Defines what each special key does
 */
void MainWindowGlut::glutSpecialKeys_cb(int key, int x, int y)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    switch(key)
    {
        case GLUT_KEY_LEFT:
            glRotatef(-3, 0.0, 1.0, 0.0);
            break;
        case GLUT_KEY_RIGHT:
            glRotatef(3, 0.0, 1.0, 0.0);
            break;
        case GLUT_KEY_UP:
            glRotatef(3, 1.0, 0.0, 0.0);
            break;
        case GLUT_KEY_DOWN:
            glRotatef(-3, 1.0, 0.0, 0.0);
            break;
        case GLUT_KEY_PAGE_UP:
            glTranslatef(0.0, 0.0, 0.3);
            break;
        case GLUT_KEY_PAGE_DOWN:
            glTranslatef(0.0, 0.0, -0.3);
            break;
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Define Mouse interaction
 */
void MainWindowGlut::glutMouse_cb(int button, int state, int x, int y)
{
    //handle mouse interaction for rotating/zooming the view
    if (state == GLUT_DOWN) {
        MainWindowGlut::mouse_buttons |= 1<<button;
    } else if (state == GLUT_UP) {
        MainWindowGlut::mouse_buttons = 0;
    }

    MainWindowGlut::mouse_old_x = x;
    MainWindowGlut::mouse_old_y = y;
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * Define motion of the coordinate system
 */
void MainWindowGlut::glutMotion_cb(int x, int y)
{
    //hanlde the mouse motion for zooming and rotating the view
    float dx, dy;
    dx = x - MainWindowGlut::mouse_old_x;
    dy = y - MainWindowGlut::mouse_old_y;

    if (MainWindowGlut::mouse_buttons & 1) {
        MainWindowGlut::rotate_x += dy * 0.2;
        MainWindowGlut::rotate_y += dx * 0.2;
    } else if (MainWindowGlut::mouse_buttons & 4) {
        MainWindowGlut::translate_z += dy * 0.1;
    }

    MainWindowGlut::mouse_old_x = x;
    MainWindowGlut::mouse_old_y = y;

    // set view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, MainWindowGlut::translate_z);
    glScalef(MainWindowGlut::scale[0], MainWindowGlut::scale[1], MainWindowGlut::scale[2]);
    glRotatef(MainWindowGlut::rotate_x, 1.0, 0.0, 0.0);
    glRotatef(MainWindowGlut::rotate_y, 0.0, 1.0, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////////

/*
 * =====================================================================================
 *       Filename:  image_acquirer_single_camera.cpp
 *    Description:  Acqure image for one camera
 *        Created:  2014-11-06 17:36
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "image_acquirer_single_camera.hpp"
#include "types.hpp"

#include <vector>

////////////////////////////////////////////////////////////////////////////////////////

ImageAcquirerSingleCamera::ImageAcquirerSingleCamera(int cap, std::string default_path) :
    _cap(new cv::VideoCapture(cap)),
    _frame(new cv::Mat()),
    _webcam_mode(true),
    _default_path(default_path)
{
    if(!_cap->isOpened()) // If no camera is available
    {
        _webcam_mode = false;
        *_frame = cv::imread(_default_path);
    }
    else {  // else use the current frame
        *_cap >> *_frame;
    }

    _width = _frame->cols;
    _height = _frame->rows;
}

////////////////////////////////////////////////////////////////////////////////////////
 
 std::vector<struct point4D<GLfloat>>& ImageAcquirerSingleCamera::vertex_coord()
{
    float max = std::max(_width, _height);
    GLfloat x,y,z;

    _vertex_coord.clear();

    for(int i=0; i < _height; i++)
    {
        for(int j=0; j < _width; j++)
        {
            x = GLfloat(j-_width/2)/max;
            y = GLfloat(i-_height/2)/max;
            z = GLfloat(0.0);
                    
            _vertex_coord.push_back(point4D<GLfloat>(x,y,z,1));
        }
    }

    return _vertex_coord;
}

////////////////////////////////////////////////////////////////////////////////////////

 std::vector<struct point4D<GLubyte>>& ImageAcquirerSingleCamera::vertex_color()
{
    cv::Vec3b pixel;

    if(_webcam_mode)
        *_cap >> *_frame;
 
    _vertex_color.clear();

    for(int i=_height-1; i >= 0; i--)
    {
        for(int j=_width-1; j >= 0; j--)
        {
            pixel = _frame->at<cv::Vec3b>(i,j);
            _vertex_color.push_back(point4D<GLubyte>(pixel[2], pixel[1], pixel[0], 255));
        }
    }

    return _vertex_color;
}

////////////////////////////////////////////////////////////////////////////////////////

#define INDEX(i,j) (i) * _width + (j)

 std::vector<struct point3D<GLuint>>& ImageAcquirerSingleCamera::vertex_index()
{
    int shift = (_height - 2) * _width + _width - 1;

    _vertex_index.clear();

    for(int i=0; i < _height-1; i++)
    {
        for(int j=0; j < _width-1; j++)
        {
            _vertex_index.push_back(point3D<GLuint>(INDEX(i,j),
                        INDEX(i,j+1),INDEX(i+1,j)));
            _vertex_index.push_back(point3D<GLuint>(INDEX(i+1,j+1),
                        INDEX(i+1,j),INDEX(i,j+1)));
        }
    }

    return _vertex_index;
}

////////////////////////////////////////////////////////////////////////////////////////

 cv::Mat ImageAcquirerSingleCamera::img()
{
    if(_webcam_mode)
        *_cap >> *_frame;

    return *_frame;
}

////////////////////////////////////////////////////////////////////////////////////////

 int ImageAcquirerSingleCamera::width()
{
    return _width;
}

////////////////////////////////////////////////////////////////////////////////////////

 int ImageAcquirerSingleCamera::height()
{
    return _height;
}

////////////////////////////////////////////////////////////////////////////////////////

bool ImageAcquirerSingleCamera::webcam_mode()
{
    return _webcam_mode;
}

////////////////////////////////////////////////////////////////////////////////////////

bool ImageAcquirerSingleCamera::set_webcam_mode(bool mode)
{
    assert(mode != _webcam_mode);
    bool suc = true;

    if(mode == false)
    {
        _cap->release();
        *_frame = cv::imread(_default_path);
        _webcam_mode = false;
        _width = _frame->cols;
        _height = _frame->rows;
    }
    else {
        _cap->open(_cap_id);
        _webcam_mode = true;

        if(!_cap->isOpened())
        {
            *_frame = cv::imread(_default_path);
            _webcam_mode = false;
            suc = false;
        }
        _width = _frame->cols;
        _height = _frame->rows;
    }

    return suc;
}

////////////////////////////////////////////////////////////////////////////////////////

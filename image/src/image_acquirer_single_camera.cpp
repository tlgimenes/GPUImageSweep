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

ImageAcquirerSingleCamera::ImageAcquirerSingleCamera(int cap) :
    _cap(new cv::VideoCapture(cap))
{
    cv::Mat frame0;

    *_cap >> frame0;

    _width = frame0.cols;
    _height = frame0.rows;
}

////////////////////////////////////////////////////////////////////////////////////////
 
 std::vector<struct point3D<GLfloat>>& ImageAcquirerSingleCamera::vertex_coord()
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
                    
            _vertex_coord.push_back(point3D<GLfloat>(x,y,z));
        }
    }

    return _vertex_coord;
}

////////////////////////////////////////////////////////////////////////////////////////

 std::vector<struct point3D<GLubyte>>& ImageAcquirerSingleCamera::vertex_color()
{
    cv::Mat frame;
    cv::Vec3b pixel;

    *_cap >> frame;
 
    _vertex_color.clear();

    for(int i=_height-1; i >= 0; i--)
    {
        for(int j=_width-1; j >= 0; j--)
        {
            pixel = frame.at<cv::Vec3b>(i,j);
            _vertex_color.push_back(point3D<GLubyte>(pixel[2], pixel[1], pixel[0]));
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
    cv::Mat frame;

    *_cap >> frame;

    return frame;
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


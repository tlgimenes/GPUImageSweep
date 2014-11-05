/*
 * =====================================================================================
 *       Filename:  image_acquirer.cpp
 *    Description:  This class interfaces with the webcam to receive the images
 *        Created:  2014-10-24 10:35
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "image_acquirer.hpp"

////////////////////////////////////////////////////////////////////////////////////////

ImageAcquirer::ImageAcquirer() :
    _cap1(new cv::VideoCapture(0)),
    _cap2(new cv::VideoCapture(0))
{
    cv::Mat frame0;

    *_cap1 >> frame0;

    _width = frame0.cols;
    _height = frame0.rows;
}

////////////////////////////////////////////////////////////////////////////////////////

cv::Mat ImageAcquirer::img1()
{
    cv::Mat frame;

    *_cap1 >> frame;

    return frame;
}

////////////////////////////////////////////////////////////////////////////////////////

cv::Mat ImageAcquirer::img2()
{
    cv::Mat frame;

    *_cap2 >> frame;

    return frame;
}

////////////////////////////////////////////////////////////////////////////////////////

int ImageAcquirer::width()
{
    return _width;
}

////////////////////////////////////////////////////////////////////////////////////////

int ImageAcquirer::height()
{
    return _height;
}

////////////////////////////////////////////////////////////////////////////////////////

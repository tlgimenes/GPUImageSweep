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
    _img1(0, DEFAULT_PATH_IMAGE_1),
    _img2(1, DEFAULT_PATH_IMAGE_2)
{
    if(_img1.webcam_mode() != _img2.webcam_mode()) 
    {
        if(_img1.webcam_mode()) _img1.set_webcam_mode(false);
        if(_img2.webcam_mode()) _img2.set_webcam_mode(false);
    }
}

////////////////////////////////////////////////////////////////////////////////////////

ImageAcquirerSingleCamera& ImageAcquirer::acquirer1()
{
    return _img1;
}

////////////////////////////////////////////////////////////////////////////////////////

ImageAcquirerSingleCamera& ImageAcquirer::acquirer2()
{
    return _img2;
}

////////////////////////////////////////////////////////////////////////////////////////

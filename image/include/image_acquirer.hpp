/*
 * =====================================================================================
 *       Filename:  image_acquirer.hpp
 *    Description:  This class interfaces with the webcam to receive the images
 *        Created:  2014-10-24 10:36
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef IMAGE_ACQUIRER_HPP
#define IMAGE_ACQUIRER_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include <opencv2/opencv.hpp>
#include <memory>

////////////////////////////////////////////////////////////////////////////////////////

class ImageAcquirer
{
    private:
        // Default camera
        std::shared_ptr<cv::VideoCapture> _cap1;
        std::shared_ptr<cv::VideoCapture> _cap2;

        int _width, _height;

    public:
        ImageAcquirer();

        cv::Mat img1();
        cv::Mat img2();
        
        int width();
        int height();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !IMAGE_ACQUIRER_HPP */

////////////////////////////////////////////////////////////////////////////////////////


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

#include "types.hpp"
#include "image_acquirer_single_camera.hpp"

#include <opencv2/opencv.hpp>
#include <memory>

#define DEFAULT_PATH_IMAGE_1 "../samples/face00R.tif"
#define DEFAULT_PATH_IMAGE_2 "../samples/face01R.tif"
//#define DEFAULT_PATH_IMAGE_1 "../samples/tiago1.png"
//#define DEFAULT_PATH_IMAGE_2 "../samples/tiago2.png"

////////////////////////////////////////////////////////////////////////////////////////

class ImageAcquirer
{
    private:
        // Default camera
        ImageAcquirerSingleCamera _img1;
        ImageAcquirerSingleCamera _img2;

        cv::Mat _K2, _Kref, _R2, _T2;

    public:
        ImageAcquirer();

        ImageAcquirerSingleCamera& acquirer1();
        ImageAcquirerSingleCamera& acquirer2();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !IMAGE_ACQUIRER_HPP */

////////////////////////////////////////////////////////////////////////////////////////


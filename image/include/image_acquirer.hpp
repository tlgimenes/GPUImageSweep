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

////////////////////////////////////////////////////////////////////////////////////////

class ImageAcquirer
{
    private:
        /**
         * Default cameras. Fallback mode with default image
         * */
        ImageAcquirerSingleCamera _img1;
        ImageAcquirerSingleCamera _img2;

    public:
        ImageAcquirer(std::string path1 = DEFAULT_PATH_IMAGE_1, 
                      std::string path2 = DEFAULT_PATH_IMAGE_2);

        /**
         * Get attributes 
         * */
        ImageAcquirerSingleCamera& acquirer1();
        ImageAcquirerSingleCamera& acquirer2();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !IMAGE_ACQUIRER_HPP */

////////////////////////////////////////////////////////////////////////////////////////


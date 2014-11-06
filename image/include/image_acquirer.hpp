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

////////////////////////////////////////////////////////////////////////////////////////

class ImageAcquirer
{
    private:
        // Default camera
        ImageAcquirerSingleCamera _img1;
        ImageAcquirerSingleCamera _img2;

    public:
        ImageAcquirer();

        cv::Mat img1();
        cv::Mat img2();

        ImageAcquirerSingleCamera& acquirer1();
        ImageAcquirerSingleCamera& acquirer2();

        std::vector<struct point3D<GLfloat>>& vertex_coord1();
        std::vector<struct point3D<GLubyte>>& vertex_color1();
        std::vector<struct point3D<GLuint>> & vertex_index1();
 
        std::vector<struct point3D<GLfloat>>& vertex_coord2();
        std::vector<struct point3D<GLubyte>>& vertex_color2();
        std::vector<struct point3D<GLuint>> & vertex_index2();

        int width();
        int height();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !IMAGE_ACQUIRER_HPP */

////////////////////////////////////////////////////////////////////////////////////////


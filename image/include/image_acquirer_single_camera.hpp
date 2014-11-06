/*
 * =====================================================================================
 *       Filename:  image_acquirer_single_camera.hpp
 *    Description:  Acquire image for a single camera
 *        Created:  2014-11-06 17:36
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef IMAGE_ACQUIRER_SINGLE_CAMERA_HPP
#define IMAGE_ACQUIRER_SINGLE_CAMERA_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include "types.hpp"

#include <GL/glew.h>
#include <GL/gl.h>
#include <opencv2/opencv.hpp>
#include <memory>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////

class ImageAcquirerSingleCamera
{
    private:
        std::shared_ptr<cv::VideoCapture> _cap;

        int _width, _height;

        std::vector<struct point3D<GLfloat>> _vertex_coord;
        std::vector<struct point3D<GLubyte>> _vertex_color;
        std::vector<struct point3D<GLuint>> _vertex_index;

    public:
        ImageAcquirerSingleCamera(int cap);

        /**
         * Return the raw cv::Mat image from
         * */
        cv::Mat img();

        /**
         * Return the std::vector of the a 3D model of the an image 
         * */
        std::vector<struct point3D<GLfloat>>& vertex_coord();

        /**
         * Returns the std::vector of the colors of an image 
         * */
        std::vector<struct point3D<GLubyte>>& vertex_color();

        /**
         * Returns the indexing of the vertex of the 3D model 
         * representing this image */
        std::vector<struct point3D<GLuint>>& vertex_index();

        /**
         * Width of the image 
         * */
        int width();

        /**
         * Height of the image 
         * */
        int height();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !IMAGE_ACQUIRER_SINGLE_CAMERA_HPP */

////////////////////////////////////////////////////////////////////////////////////////


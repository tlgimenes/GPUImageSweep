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
        /* Attribute used when webcam is available for retrieving images */
        std::shared_ptr<cv::VideoCapture> _cap;

        /* Current frame of the capturing device */
        std::shared_ptr<cv::Mat> _frame;

        int _width, _height;

        /**
         * 3D model attributes 
         * */
        std::vector<struct point4D<GLfloat>> _vertex_coord;
        std::vector<struct point4D<GLubyte>> _vertex_color;
        std::vector<struct point3D<GLuint>> _vertex_index;

        /* Wheter webcam is available or not */
        bool _webcam_mode;

        /* Default path for the default picture in case of webcam inexistence */
        std::string _default_path;

        /* ID of the capture device */
        int _cap_id;

    public:
        ImageAcquirerSingleCamera(int cap, std::string default_path);

        /**
         * Return the raw cv::Mat image from
         * */
        cv::Mat img();

        /**
         * Return the std::vector of the a 3D model of the an image 
         * */
        std::vector<struct point4D<GLfloat>>& vertex_coord();

        /**
         * Returns the std::vector of the colors of an image 
         * */
        std::vector<struct point4D<GLubyte>>& vertex_color();

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

        /**
         * Gets the webcam_mode
         * */
        bool webcam_mode();

        /**
         * Sets webcam mode on/off. 
         * @params: mode: True enables webcam.
         *                False disables webcam 
         * @returns: True if mode change was possible, false otherwise
         * WARINING ! Be carefull with this function ! Just use it before
         * calling vertex_{coor,color,index} since the _width and _height of
         * the image will change as well as the buffer
         * */ 
        bool set_webcam_mode(bool mode);
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !IMAGE_ACQUIRER_SINGLE_CAMERA_HPP */

////////////////////////////////////////////////////////////////////////////////////////


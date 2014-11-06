/*
 * =====================================================================================
 *       Filename:  clgl_image.hpp
 *    Description:  Class for creating and loading an image from the image
 *    acquirer into the GPU. This class uses 3 vbos, one for vertex coordinates
 *    of the 3D model, one vbo for colors and a third one for indexing the vbos
 *        Created:  2014-11-06 19:30
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef CLGL_IMAGE_HPP
#define CLGL_IMAGE_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include "image_acquirer_single_camera.hpp"
#include "clgl.hpp"

////////////////////////////////////////////////////////////////////////////////////////

class CLGLImage
{
    private:
        int _vertex_coord_vbo_id, _vertex_color_vbo_id, _vertex_index_vbo_id;

        ImageAcquirerSingleCamera& _img;

        CLGL& _clgl;

        int _num_index_elements;

    public:
        CLGLImage(CLGL& clgl, ImageAcquirerSingleCamera& img);

        /**
         * Updates the color of the image. 
         * This method basicaly gets a new image from the image acquirer 
         * and sets update the color vbo in the GPU 
         * */
        void update();

        /**
         * Gets the VBOs ids of coordinates, colors and indices
         * */
        int vertex_coord_vbo_id();
        int vertex_color_vbo_id();
        int vertex_index_vbo_id();

        /**
         * Gets the number of indexing elements 
         * */
        int num_index_elements();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !CLGL_IMAGE_HPP */

////////////////////////////////////////////////////////////////////////////////////////


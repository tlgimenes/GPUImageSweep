/*
 * =====================================================================================
 *       Filename:  clgl_image.cpp
 *    Description:  CLGL image definitions
 *        Created:  2014-11-06 19:31
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_image.hpp"

////////////////////////////////////////////////////////////////////////////////////////

CLGLImage::CLGLImage(CLGL& clgl, ImageAcquirerSingleCamera& img) :
    _img(img),
    _clgl(clgl)
{   
    std::vector<point4D<GLfloat>>& vertx = _img.vertex_coord();
    std::vector<point4D<GLubyte>>& color = _img.vertex_color();
    std::vector<point3D<GLuint>>&  index = _img.vertex_index();

    _num_index_elements = index.size()*3;

    if(vertx.size() > 0 && color.size() > 0 && index.size() > 0)
    {
        std::cout << "Pushing " << vertx.size() << " vertex coordinates to device" << std::endl;
        _vertex_coord_vbo_id = clgl.clgl_load_vbo_data_to_device<GL_ARRAY_BUFFER>(
                sizeof(point4D<GLfloat>)*vertx.size(), 
                (const void*)vertx.data(), CL_MEM_READ_WRITE);

        std::cout << "Pushing " << color.size() << " vertex colors to device" << std::endl;
        _vertex_color_vbo_id = clgl.clgl_load_vbo_data_to_device<GL_ARRAY_BUFFER>(
                sizeof(point4D<GLubyte>)*color.size(),
                (const void*)color.data(), CL_MEM_READ_WRITE);

        _vertex_index_vbo_id = clgl.clgl_load_vbo_data_to_device<GL_ELEMENT_ARRAY_BUFFER>(
                sizeof(point3D<GLuint>)*index.size(), 
                (const void*)index.data(), CL_MEM_READ_ONLY);
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void CLGLImage::update()
{
    std::vector<point4D<GLubyte>>& color = _img.vertex_color();

    if(color.size() > 0)
    {
        _clgl.clgl_load_vbo_data_to_device(_vertex_color_vbo_id, 
                sizeof(point4D<GLubyte>)*color.size(), color.data());
    }
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLImage::vertex_coord_vbo_id()
{
    return _vertex_coord_vbo_id;
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLImage::vertex_color_vbo_id()
{
    return _vertex_color_vbo_id;
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLImage::vertex_index_vbo_id()
{
    return _vertex_index_vbo_id;
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLImage::num_index_elements()
{
    return _num_index_elements;
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLImage::width()
{
    return _img.width();
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLImage::height()
{
    return _img.height();
}

////////////////////////////////////////////////////////////////////////////////////////

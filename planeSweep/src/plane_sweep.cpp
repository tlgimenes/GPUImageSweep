/*
 * =====================================================================================
 *       Filename:  plane_sweep.cpp
 *    Description:  File containning calls for running the plane_sweep kernel 
 *        Created:  2014-11-06 22:40
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "plane_sweep.hpp"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////////////

PlaneSweep::PlaneSweep(CLGL& clgl, CLGLProjection& h, CLGLImage& img1, CLGLImage& img2) : 
    _clgl(clgl)
{
    _n_threads_compute_plane_kernel = img1.height() * img1.width();
    _n_threads_project_plane_kernel = _n_threads_compute_plane_kernel;
    assert(_n_threads_compute_plane_kernel > 0);

    std::string programName = "../kernels/plane_sweep.cl";
    std::string compilerFlags = "-Werror -cl-std=CL1.1";
    std::string compute_plane_kernel_name = "compute_plane";
    std::string project_plane_kernel_name = "project_plane";

    _clgl.clgl_build_program_source(programName, compilerFlags);
    _compute_plane_kernel_id = _clgl.clgl_build_kernel(compute_plane_kernel_name);
    _project_plane_kernel_id = _clgl.clgl_build_kernel(project_plane_kernel_name);

    create_plane_index_buffer(img1.height() * img1.width()); // one plane per pixel

    int n_planes = h.n_planes();
    int height = img1.height();
    int width = img1.width();
    float d_min = h.d_min();

    std::cout << "n_planes " << n_planes << std::endl;
    std::cout << "height " << height << std::endl;
    std::cout << "width " << width << std::endl;

    _clgl.clgl_set_arg<CLGL_CL>(0, h.buffer_id_homography(), _compute_plane_kernel_id);
    _clgl.clgl_set_arg<CLGL_VBO>(1, img1.vertex_color_vbo_id(), _compute_plane_kernel_id);
    _clgl.clgl_set_arg<CLGL_VBO>(2, img2.vertex_color_vbo_id(), _compute_plane_kernel_id);
    //_clgl.clgl_set_arg<CLGL_CL>(3, _buffer_id_plane_index, _compute_plane_kernel_id);
    _clgl.clgl_set_arg<CLGL_VBO>(3, img1.vertex_coord_vbo_id(), _compute_plane_kernel_id);
    _clgl.clgl_set_arg(4, sizeof(int), &n_planes , _compute_plane_kernel_id);
    _clgl.clgl_set_arg(5, sizeof(int), &height, _compute_plane_kernel_id);
    _clgl.clgl_set_arg(6, sizeof(int), &width , _compute_plane_kernel_id);

    /*_clgl.clgl_set_arg<CLGL_CL>(0, , _project_plane_kernel_id);
    _clgl.clgl_set_arg<CLGL_VBO>(1, img1.vertex_color_vbo_id(), _project_plane_kernel_id);
    _clgl.clgl_set_arg<CLGL_CL>(2, , _project_plane_kernel_id);
    _clgl.clgl_set_arg<CLGL_VBO>(3, img1.vertex_coord_vbo_id(), _project_plane_kernel_id);
    _clgl.clgl_set_arg(4, sizeof(int), &height, _project_plane_kernel_id);
    _clgl.clgl_set_arg(5, sizeof(int), &width , _project_plane_kernel_id);
    */
    
    _clgl.clgl_set_arg<CLGL_CL>(0, _buffer_id_plane_index, _project_plane_kernel_id);
    _clgl.clgl_set_arg<CLGL_VBO>(1, img1.vertex_coord_vbo_id(), _project_plane_kernel_id);
    _clgl.clgl_set_arg(2, sizeof(uint), &height, _project_plane_kernel_id);
    _clgl.clgl_set_arg(3, sizeof(uint), &width , _project_plane_kernel_id);
    _clgl.clgl_set_arg(4, sizeof(float), &d_min , _project_plane_kernel_id);
    _clgl.clgl_set_arg(5, sizeof(uint), &n_planes , _project_plane_kernel_id);
}

////////////////////////////////////////////////////////////////////////////////////////

void PlaneSweep::run_compute_plane_kernel()
{
    std::cout << "running kernel with " << _n_threads_compute_plane_kernel << " threads" << std::endl;
    _clgl.clgl_run_kernel(_compute_plane_kernel_id, _n_threads_compute_plane_kernel);
}

////////////////////////////////////////////////////////////////////////////////////////

void PlaneSweep::run_project_plane_kernel()
{
    std::cout << "running kernel with " << _n_threads_project_plane_kernel << " threads" << std::endl;
    _clgl.clgl_run_kernel(_project_plane_kernel_id, _n_threads_project_plane_kernel);
}

////////////////////////////////////////////////////////////////////////////////////////

void PlaneSweep::create_plane_index_buffer(int size)
{
    assert(size > 0);
    uint* data = new uint[size];
    memset(data, 0, sizeof(uint) * size);

    std::cout << "pushing " << size << " uints into the GPU" << std::endl;
    _buffer_id_plane_index = _clgl.clgl_load_data_to_device(CL_TRUE, sizeof(uint)*size, data,
            CL_MEM_READ_WRITE);
}

////////////////////////////////////////////////////////////////////////////////////////

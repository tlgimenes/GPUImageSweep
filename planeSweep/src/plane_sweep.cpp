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

PlaneSweep::PlaneSweep(CLGL& clgl, Homography& h, CLGLImage& img1, CLGLImage& img2) : 
    _clgl(clgl)
{
    _n_threads_plane_sweep_kernel = img1.height() * img1.width();
    assert(_n_threads_plane_sweep_kernel > 0);

    std::string programName = "../kernels/plane_sweep.cl";
    std::string compilerFlags = "-Werror -cl-std=CL1.1";
    std::string plane_sweep_kernel_name = "plane_sweep";

    _clgl.clgl_build_program_source(programName, compilerFlags);
    _plane_sweep_kernel_id = _clgl.clgl_build_kernel(plane_sweep_kernel_name);

    int n_planes = h.n_planes();
    int height = img1.height();
    int width = img1.width();
    float d_min = h.d_min();
    float delta = h.delta();

    std::cout << "n_planes " << n_planes << std::endl;

    _clgl.clgl_set_arg<CLGL_CL>(0, h.buffer_id(), _plane_sweep_kernel_id);
    _clgl.clgl_set_arg<CLGL_VBO>(1, img1.vertex_color_vbo_id(), _plane_sweep_kernel_id);
    _clgl.clgl_set_arg<CLGL_VBO>(2, img2.vertex_color_vbo_id(), _plane_sweep_kernel_id);
    _clgl.clgl_set_arg<CLGL_VBO>(3, img1.vertex_coord_vbo_id(), _plane_sweep_kernel_id);
    _clgl.clgl_set_arg(4, sizeof(int), &n_planes , _plane_sweep_kernel_id);
    _clgl.clgl_set_arg(5, sizeof(int), &height, _plane_sweep_kernel_id);
    _clgl.clgl_set_arg(6, sizeof(int), &width , _plane_sweep_kernel_id);
    _clgl.clgl_set_arg(7, sizeof(float), &d_min , _plane_sweep_kernel_id);
    _clgl.clgl_set_arg(8, sizeof(float), &delta , _plane_sweep_kernel_id);
}

////////////////////////////////////////////////////////////////////////////////////////

void PlaneSweep::run_plane_sweep_kernel()
{
    std::cout << "running kernel wit " << _n_threads_plane_sweep_kernel << " threads" << std::endl;
    _clgl.clgl_run_kernel(_plane_sweep_kernel_id, _n_threads_plane_sweep_kernel);
}

////////////////////////////////////////////////////////////////////////////////////////


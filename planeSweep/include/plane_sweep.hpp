/*
 * =====================================================================================
 *       Filename:  plane_sweep.hpp
 *    Description:  
 *        Created:  2014-11-06 22:41
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef PLANE_SWEEP_HPP
#define PLANE_SWEEP_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl.hpp"
#include "clgl_projection.hpp"
#include "clgl_image.hpp"

////////////////////////////////////////////////////////////////////////////////////////

class PlaneSweep
{
    private:
        CLGL& _clgl;

        /* ID's of different kernels used in the plane sweep algorithm */
        int _compute_plane_kernel_id; // ID of the compute_plane kernel
        int _project_plane_kernel_id; // ID of the project_plane kernel

        /**
         *  Buffer containing the index of each chosen plane for each pixel 
         * */
        int _buffer_id_plane_index; 

        /**
         * Number of threads to launch for each kernel  
         * */
        int _n_threads_compute_plane_kernel; // number of threads to launch for the compute_plane kernel
        int _n_threads_project_plane_kernel; // number of threads to launch for the project_plane kernel

    protected:
        /**
         * Creates a buffer in the GPU of uints of size uints
         * */
        void create_plane_index_buffer(int size);

    public:
        PlaneSweep(CLGL& clgl, CLGLProjection& H, CLGLImage& img1, CLGLImage& img2);

        /**
         * Runs each kernel  
         * */
        void run_compute_plane_kernel(); // runs the compute_plane kernel
        void run_project_plane_kernel(); // runs the project_plane kernel
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !PLANE_SWEEP_HPP */

////////////////////////////////////////////////////////////////////////////////////////


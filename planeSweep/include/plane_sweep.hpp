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
#include "homography.hpp"
#include "clgl_image.hpp"

////////////////////////////////////////////////////////////////////////////////////////

class PlaneSweep
{
    private:
        CLGL& _clgl;

        int _plane_sweep_kernel_id;

        int _n_threads_plane_sweep_kernel;

    public:
        PlaneSweep(CLGL& clgl, Homography& H, CLGLImage& img1, CLGLImage& img2);

        void run_plane_sweep_kernel();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !PLANE_SWEEP_HPP */

////////////////////////////////////////////////////////////////////////////////////////


/*
 * =====================================================================================
 *       Filename:  clgl_projection.hpp
 *    Description:  
 *        Created:  2014-11-06 20:57
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef CLGL_HOMOGRAPHY_HPP
#define CLGL_HOMOGRAPHY_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include "Eigen/Dense"
#include "clgl.hpp"
#include "types.hpp"

#include <opencv2/opencv.hpp>

////////////////////////////////////////////////////////////////////////////////////////

class CLGLProjection
{
    private:
        /**
         * Matrix for calculating the homography. The names of the variables
         * follows the reference paper
         * */
        Eigen::Matrix3f _A1, _A2;   // cameraMatrix * rotation
        Eigen::Vector3f _B1, _B2;   // cameraMatrix * translation
        Eigen::Vector3f _C1, _C2;   // center of the camera
        Eigen::Vector3f _n; // normal of the projection plane

        /**
         * Minimum and maximum distance for the planes
         * */
        float _d_min, _d_max;

        /**
         * Buffer id of matrices loadded in GPU memory
         * */
        int _buffer_id_homography; // buffer id of homography matrices Image1 -> Image2
        int _buffer_id_projection; // buffer id of projection matrices Image1 -> 3D Object

        /**
         * Number of planes of the buffer loadded in GPU memory
         * */
        int _n_planes;

        /**
         * Distance between planes 
         * */
        float _delta;

    protected:
        /**
         * Calculates the homography matrix (3x3) for the distance d_k 
         *  */
        void homography_for_dist(float d_k, std::vector<point4D<GLfloat>>& H_k);

        /**
         * Calculates the projection matrix (3x4) for the distance d_k 
         * */
        void projection_for_dist(float d_k, std::vector<point4D<GLfloat>>& P_k);

    public:
        /** 
         * Calculates all homography matrices from distance d_min and d_max,
         * store all matrices in a buffer and load it to the GPU
         * */
        CLGLProjection(cv::Matx33d& A1, cv::Vec3d& B1, cv::Matx33d& A2, cv::Vec3d& B2, 
                float d_min, float d_max, int n_planes, CLGL& clgl);

        /**
         * Buffer id of the vbo representing the homography matrix 
         * in the GPU
         * */
        int buffer_id_homography();

        /**
         * Buffer id of the vbo representing the projection matrix 
         * in the GPU 
         * */
        int buffer_id_projection();

        /**
         * Number of planes (number of homography matrices) in the 
         * device (GPU) 
         * */
        int n_planes();

        /**
         * Closest distance of a plane 
         * */
        float d_min();

        /**
         * Distance between planes 
         * */
        float delta();
};

////////////////////////////////////////////////////////////////////////////////////////

#endif /* !CLGL_HOMOGRAPHY_HPP */

////////////////////////////////////////////////////////////////////////////////////////


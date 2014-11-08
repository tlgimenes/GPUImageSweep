/*
 * =====================================================================================
 *       Filename:  homography.hpp
 *    Description:  
 *        Created:  2014-11-06 20:57
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#ifndef HOMOGRAPHY_HPP
#define HOMOGRAPHY_HPP

////////////////////////////////////////////////////////////////////////////////////////

#include "Eigen/Dense"
#include "clgl.hpp"
#include "types.hpp"

#include <opencv2/opencv.hpp>

////////////////////////////////////////////////////////////////////////////////////////

class Homography
{
    private:
        /**
         * Matrix for calculating the homography. The names of the variables
         * follows the reference paper
         * */
        Eigen::Matrix3f _Kref_inv; // The inverse of the reference camera matrix;
        Eigen::Matrix3f _K2;  // The camera matrix of the second camera
        Eigen::Matrix3f _R2;  // Rotation of the second camera
        Eigen::Vector3f _T2;  // Translation of the second camera
        Eigen::Vector3f _n_k; // Normal vector of the planes

        /**
         * Minimum and maximum distance for the planes
         * */
        float _d_min, _d_max;

        /**
         * Buffer id of the homography matrix loadded in GPU memory
         * */
        int _buffer_id;

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

    public:
        /** 
         * Calculates all homography matrices from distance d_min and d_max,
         * store all matrices in a buffer and load it to the GPU
         * */
        Homography(cv::Mat& K2, cv::Mat& Kref, cv::Mat& R2, cv::Mat& T2, 
                float d_min, float d_max, int n_planes, CLGL& clgl);

        /**
         * Buffer id of the vbo representing the homography matrix 
         * int the GPU
         * */
        int buffer_id();

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

#endif /* !HOMOGRAPHY_HPP */

////////////////////////////////////////////////////////////////////////////////////////


/*
 * =====================================================================================
 *       Filename:  homography.cpp
 *    Description:  Class for loading the homography matrices into the GPU
 *        Created:  2014-11-06 20:56
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "homography.hpp"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////////////

/* Converts a 3x3 float cv::Mat to an Eigen::matrix3f */
void cv_mat_to_eigen_matrix3f(cv::Mat& cv_mat, Eigen::Matrix3f& eigen_mat);
/* Converts a 3x1 float cv::Mat to an Eigen::Vector3f */
void cv_mat_to_eigen_vector3f(cv::Mat& cv_mat, Eigen::Vector3f& eigen_mat);

////////////////////////////////////////////////////////////////////////////////////////

Homography::Homography(cv::Mat& K2, cv::Mat& Kref, cv::Mat& R2, cv::Mat& T2, 
        float d_min, float d_max, int n_planes, CLGL& clgl) :
    _n_planes(n_planes),
    _d_min(d_min),
    _delta((d_max-d_min) / n_planes),
    _n_k(0,0,1)
{
    assert(n_planes > 0 && d_min < d_max && d_min > 0);

    cv_mat_to_eigen_matrix3f(K2, _K2);
    cv_mat_to_eigen_matrix3f(Kref, _Kref_inv);
    cv_mat_to_eigen_matrix3f(R2, _R2);
    cv_mat_to_eigen_vector3f(T2, _T2);

    // _Kerf_inv has untill now the value of Kref, so we need to invert it
    _Kref_inv.inverse(); 

    std::vector<point4D<GLfloat>> H;
    std::vector<point4D<GLfloat>> H_k;
    float d_k;

    for(int i=1; i <= n_planes; i++)
    {
        d_k = d_min + _delta * i;
        H_k.clear();
        this->homography_for_dist(d_k, H_k);
        H.insert(H.end(), H_k.begin(), H_k.end());
    }

    std::cout << "pushing " << H.size() << " matrix lines to the GPU" << std::endl;
    _buffer_id = clgl.clgl_load_data_to_device(CL_TRUE, sizeof(point4D<GLfloat>)*H.size(),
            (const void*)H.data(), CL_MEM_READ_ONLY);
}

////////////////////////////////////////////////////////////////////////////////////////

void Homography::homography_for_dist(float d_k, std::vector<point4D<GLfloat>>& H_k)
{
    Eigen::Matrix3f H = _K2 * (_R2 + _T2 * _n_k.transpose() / d_k) * _Kref_inv;

    H_k.push_back(point4D<GLfloat>(H(0,0),H(0,1),H(0,2), 0));
    H_k.push_back(point4D<GLfloat>(H(1,0),H(1,1),H(1,2), 0));
    H_k.push_back(point4D<GLfloat>(H(2,0),H(2,1),H(2,2), 0));
}

////////////////////////////////////////////////////////////////////////////////////////

int Homography::buffer_id()
{
    return _buffer_id;
}

////////////////////////////////////////////////////////////////////////////////////////

int Homography::n_planes()
{
    return _n_planes;
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Closest distance of a plane 
 * */
float Homography::d_min()
{
    return _d_min;
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Distance between planes 
 * */
float Homography::delta()
{
    return _delta;
}

////////////////////////////////////////////////////////////////////////////////////////

void cv_mat_to_eigen_matrix3f(cv::Mat& cv_mat, Eigen::Matrix3f& eigen_mat)
{
    // cv_mat must be a 3x3 matrix
    assert(cv_mat.rows == 3 && cv_mat.cols == 3);

    for(int i=0; i < 3; i++)
    {
        for(int j=0; j < 3; j++)
        {
            eigen_mat(i,j) = cv_mat.at<float>(i,j);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void cv_mat_to_eigen_vector3f(cv::Mat& cv_mat, Eigen::Vector3f& eigen_mat)
{
    // cv_mat must be a 3x1 matrix
    assert(cv_mat.rows == 3 && cv_mat.cols == 1);

    eigen_mat(0,0) = cv_mat.at<float>(0,0);
    eigen_mat(1,0) = cv_mat.at<float>(1,0);
    eigen_mat(2,0) = cv_mat.at<float>(2,0);
}

////////////////////////////////////////////////////////////////////////////////////////


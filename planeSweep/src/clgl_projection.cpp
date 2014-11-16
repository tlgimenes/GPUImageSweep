/*
 * =====================================================================================
 *       Filename:  homography.cpp
 *    Description:  Class for loading the homography matrices into the GPU
 *        Created:  2014-11-06 20:56
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#include "clgl_projection.hpp"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////////////

/* Converts a 3x3 float cv::Mat to an Eigen::matrix3f */
void cv_mat_to_eigen_matrix3f(cv::Matx33d& cv_mat, Eigen::Matrix3f& eigen_mat);
/* Converts a 3x1 float cv::Mat to an Eigen::Vector3f */
void cv_mat_to_eigen_vector3f(cv::Vec3d& cv_mat, Eigen::Vector3f& eigen_mat);

////////////////////////////////////////////////////////////////////////////////////////

CLGLProjection::CLGLProjection(cv::Matx33d& A1, cv::Vec3d& B1, cv::Matx33d& A2, cv::Vec3d& B2, 
        float d_min, float d_max, int n_planes, CLGL& clgl) :
    _n_planes(n_planes),
    _d_min(d_min),
    _delta((d_max-d_min) / n_planes)
{
    assert(n_planes > 0 && d_min < d_max && d_min > 0.0f);

    cv_mat_to_eigen_matrix3f(A1, _A1);
    cv_mat_to_eigen_matrix3f(A2, _A2);
    cv_mat_to_eigen_vector3f(B1, _B1);
    cv_mat_to_eigen_vector3f(B2, _B2);

    _n = (_A1.block(2,0,1,3)).transpose(); // Last line of matrix A1
    _n.normalize();           // Normalizes the normal

    _C1 = -_A1.inverse()*_B1;
    _C2 = -_A2.inverse()*_B2;

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

    for(int i=0; i < H.size(); i+=3){
        std::cout << "[[" << H[i].x() << "," << H[i].y() << "," << H[i].z() << "," << H[i].w() << "]," << std::endl;
        std::cout << " [" << H[i+1].x() << "," << H[i+1].y() << "," << H[i+1].z() << "," << H[i+1].w() << "]," << std::endl;
        std::cout << " [" << H[i+2].x() << "," << H[i+2].y() << "," << H[i+2].z() << "," << H[i+2].w() << "]]" << std::endl << std::endl;
    }std::cout << std::endl;

    std::cout << "pushing " << H.size() << " matrix lines to the GPU" << std::endl;
    _buffer_id_homography = clgl.clgl_load_data_to_device(CL_TRUE, 
            sizeof(point4D<GLfloat>)*H.size(),(const void*)H.data(), CL_MEM_READ_ONLY);
}

////////////////////////////////////////////////////////////////////////////////////////

void CLGLProjection::projection_for_dist(float d_k, std::vector<point4D<GLfloat>>& P_k)
{
    return;
}

////////////////////////////////////////////////////////////////////////////////////////

void CLGLProjection::homography_for_dist(float d_k, std::vector<point4D<GLfloat>>& H_k)
{
    double tmp = _n.transpose() * (_A1.inverse() * _B1 + _C1);
    double lambda = tmp + d_k;
       
    Eigen::Matrix3f H;

    H = lambda * _A1.inverse();
    H = H - _A1.inverse() * _B1 * _n.transpose() * _A1.inverse();
    H = _A2 * H;
    H = H + _B2 * _n.transpose() * _A1.inverse();

    H_k.push_back(point4D<GLfloat>(H(0,0),H(0,1),H(0,2), 0));
    H_k.push_back(point4D<GLfloat>(H(1,0),H(1,1),H(1,2), 0));
    H_k.push_back(point4D<GLfloat>(H(2,0),H(2,1),H(2,2), 0));
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLProjection::buffer_id_homography()
{
    return _buffer_id_homography;
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLProjection::buffer_id_projection()
{
    return _buffer_id_projection;
}

////////////////////////////////////////////////////////////////////////////////////////

int CLGLProjection::n_planes()
{
    return _n_planes;
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Closest distance of a plane 
 * */
float CLGLProjection::d_min()
{
    return _d_min;
}

////////////////////////////////////////////////////////////////////////////////////////

/**
 * Distance between planes 
 * */
float CLGLProjection::delta()
{
    return _delta;
}

////////////////////////////////////////////////////////////////////////////////////////

void cv_mat_to_eigen_matrix3f(cv::Matx33d& cv_mat, Eigen::Matrix3f& eigen_mat)
{
    // cv_mat must be a 3x3 matrix
    assert(cv_mat.rows == 3 && cv_mat.cols == 3);

    for(int i=0; i < 3; i++)
    {
        for(int j=0; j < 3; j++)
        {
            eigen_mat(i,j) = cv_mat(i,j);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void cv_mat_to_eigen_vector3f(cv::Vec3d& cv_mat, Eigen::Vector3f& eigen_mat)
{
    // cv_mat must be a 3x1 matrix
    assert(cv_mat.rows == 3 && cv_mat.cols == 1);

    eigen_mat(0,0) = cv_mat(0);
    eigen_mat(1,0) = cv_mat(1);
    eigen_mat(2,0) = cv_mat(2);
}

////////////////////////////////////////////////////////////////////////////////////////


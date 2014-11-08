/*
 * =====================================================================================
 *       Filename:  plane_sweep.cl
 *    Description:  File containing OpenCL kernels for the plane sweep algorithm
 *        Created:  2014-11-06 22:04
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

int index(int i, int j, int width)
{
    return ((i) * width) + j;
}

////////////////////////////////////////////////////////////////////////////////////////

float cost(
    __global uchar4* image1,
    __global uchar4* image2, 
    uint2 im1, 
    uint2 im2, 
    int n, 
    int width)
{
    float cout=0;
    float4 pixel1, pixel2;

    pixel1.w = pixel2.w = 0;

    for(int i=-n; i <= n; i++)
    {
        for(int j=-n; j <= n; j++)
        {
            pixel1.x = image1[index(im1.x+i,im1.y+j,width)].x;
            pixel1.y = image1[index(im1.x+i,im1.y+j,width)].y;
            pixel1.z = image1[index(im1.x+i,im1.y+j,width)].z;

            pixel2.x = image2[index(im2.x+i,im2.y+j,width)].x;
            pixel2.y = image2[index(im2.x+i,im2.y+j,width)].y;
            pixel2.z = image2[index(im2.x+i,im2.y+j,width)].z;

            cout += fast_distance(pixel1, pixel2);
        }
    }
    return cout;
}

////////////////////////////////////////////////////////////////////////////////////////

__kernel void plane_sweep(
    __constant float4 * homography, // Homography matrix
    __global uchar4 * image1,       // RGB color image(width,height)
    __global uchar4 * image2,       // RGB color image(width,height)
    __global float4 * model_3D,     // model of size width x height elements
    int homography_size,            // number of matrix 3x3
    int height,                     // height of the image
    int width,                      // width of the image
    float d_min,                    // minimal distance of planes
    float delta)                    // distance between each plane  
{
    // Get Process Global ID
    __private uint curr = get_global_id(0);

    __private float4 m1;    // point containning the space coordinate of the point in image1
    __private float4 m2;    // point containning the space coordinate of the point in image2

    __private uint2 im1;    // point containning the coordinate of the pixel in image1
    __private uint2 im2;    // point containning the coordinate of the pixel in image2

    __private float min_cost = MAXFLOAT; // cost 
    __private float curr_cost;
        
    __private uint i, j;    // variables for interating in plane

    if(curr < height*width)
    {
        // linear indexing to (x,y) matrix indexing for dealing with images
        im1.x = curr % width;
        im1.y = curr - (m1.x * width);
        
        // space coordinates of the points
        m1.x = im1.x;
        m1.y = im1.y;
        m1.z = 1;
        m1.w = 0;

        for(i=0; i < homography_size; i++)
        {
            j = 3 * i;
            m2.x = dot(homography[j  ], m1);
            m2.y = dot(homography[j+1], m1);
            m2.z = dot(homography[j+2], m1);

            im2.x = m2.x / m2.z;
            im2.y = m2.y / m2.z;

            curr_cost = cost(image1, image2, im1, im2, 1, width);
            if(min_cost > curr_cost)
            {
                min_cost = curr_cost;
                model_3D[curr].z = i*delta + d_min;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////


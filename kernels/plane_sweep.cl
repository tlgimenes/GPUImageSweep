/*
 * =====================================================================================
 *       Filename:  plane_sweep.cl
 *    Description:  File containing OpenCL kernels for the plane sweep algorithm
 *        Created:  2014-11-06 22:04
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

////////////////////////////////////////////////////////////////////////////////////////

#define N 1
#define BETA 0.84

////////////////////////////////////////////////////////////////////////////////////////

int index(int x, int y, int width)
{
    return ((y) * width) + x;
}

////////////////////////////////////////////////////////////////////////////////////////

// Correlation
float mean(
    __constant uchar4 * image, 
    __private uint2 m,
    uint width,
    uint n) 
{
	__private float s=0;

	for (int j=-n;j<=n;j++) 
    {
		for (int i=-n;i<=n;i++) 
        {
            s += image[index(m.x+i,m.y+j,width)].x;
        }
    }
    return s/(2*n+1)/(2*n+1);
}

////////////////////////////////////////////////////////////////////////////////////////

float corr(
    __constant uchar4 * image1,
    __constant uchar4 * image2,
    __private uint2 m1,
    __private uint2 m2,
    uint width,
    uint n)
{
	__private float M1 = mean(image1, m1, width, n);
	__private float M2 = mean(image2, m2, width, n);
	__private float rho = 0;
    __private float px1 = 0, px2 = 0;

	for (int j=-n;j<=n;j++)
    {
		for (int i=-n;i<=n;i++) 
        {
            px1 = image1[index(m1.x+i,m1.y+j,width)].x;
            px2 = image2[index(m2.x+i,m2.y+j,width)].x;

            rho += (px1 - M1) * (px2 - M2);
		}
    }
    return rho;
}

////////////////////////////////////////////////////////////////////////////////////////

float NCC(
    __constant uchar4 * image1,
    __constant uchar4 * image2,
    __private uint2 m1, 
    __private uint2 m2,
    uint width,
    uint n)
{
    float cor = 0;

	float c1=corr(image1, image1, m1, m1, width, n);
	float c2=corr(image2, image2, m2, m2, width, n);

	return sqrt(c1*c2) / corr(image1, image2, m1, m2, width, n);
}

////////////////////////////////////////////////////////////////////////////////////////

float cost(
    __constant uchar4* image1,
    __constant uchar4* image2, 
    __private uint2 im1, 
    __private uint2 im2, 
    int width, 
    int n)
{
    __private float cost = 0;
    __private float4 pixel1 = 0, pixel2 = 0;

    for(int i=-n; i <= n; i++)
    {
        for(int j=-n; j <= n; j++)
        {
            pixel1.x = image1[index(im1.x+i,im1.y+j,width)].x;
            pixel1.y = image1[index(im1.x+i,im1.y+j,width)].y;
            pixel1.z = image1[index(im1.x+i,im1.y+j,width)].z;

            pixel2.x = BETA*image2[index(im2.x+i,im2.y+j,width)].x;
            pixel2.y = BETA*image2[index(im2.x+i,im2.y+j,width)].y;
            pixel2.z = BETA*image2[index(im2.x+i,im2.y+j,width)].z;

            cost += fast_distance(pixel1, pixel2);
        }
    }
    return cost;
}

////////////////////////////////////////////////////////////////////////////////////////

__kernel void compute_plane (
    __constant float4 * homography, // Homography matrix
    __constant uchar4 * image1,     // RGB color image(width,height)
    __constant uchar4 * image2,     // RGB color image(width,height)
    __global uint * plane_index,    // index of each plane
    uint homography_size,           // number of matrices 3x3
    uint height,                    // height of the image
    uint width)                     // width of the image
{
    // Get Process Global ID
    __private uint curr = get_global_id(0);

    __private float4 m1;    // point containning the space coordinate of the point in image1
    __private float4 m2;    // point containning the space coordinate of the point in image2

    __private uint2 im1;    // point containning the coordinate of the pixel in image1
    __private uint2 im2;    // point containning the coordinate of the pixel in image2

    __private float min_cost = MAXFLOAT; // cost 
    __private float curr_cost;
        
    __private uint i, j, plane_id;    // variables for interating in plane_sweep
 
    // linear indexing to (x,y) matrix indexing for dealing with imaconstant  
    im1.y = curr / width;
    im1.x = curr - width * im1.y;

    // coordinates must be with range: im1.x in (0,width-1) and im1.y in (0,height-1)
    if(N-1 < im1.x && im1.x < width-N && N-1 < im1.y && im1.y < height-N)
    {
        // space coordinates of the points
        m1.x = im1.x;
        m1.y = im1.y;
        m1.z = 1;
        m1.w = 0;

        plane_id = 0;
        for(i=0; i < homography_size; i++)
        {
            j = 3 * i;
            m2.x = dot(homography[ j ], m1);
            m2.y = dot(homography[j+1], m1);
            m2.z = dot(homography[j+2], m1);

            im2.x = m2.x / m2.z;
            im2.y = m2.y / m2.z;

            if(N-1 < im2.x && im2.x < width-N && N-1 < im2.y && im2.y < height-N) // coordinates must be valid
            {
                curr_cost = cost(image1, image2, im1, im2, width, N);
                //curr_cost = NCC(image1, image2, im1, im2, width, N);
                if(min_cost > curr_cost)
                {
                    min_cost = curr_cost;
                    plane_id = i;
                }
            }
        }
        plane_index[curr] = plane_id;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

__kernel void project_plane (
    __global uint * plane_index,    // plane index calculated by the compute_plane kernel
    __global float4 * model3D,      // 3D model
    uint height,                     // width of the image1 
    uint width,                      // heigth of the image1
    float d_min,
    uint n_planes)
{
    __private uint curr = get_global_id(0);

    __private uint2 im1;    // point containning the coordinate of the pixel in image1
 
    // linear indexing to (x,y) matrix indexing for dealing with images
    im1.y = curr / width;
    im1.x = curr - width * im1.y;

    // coordinates must be with range: im1.x in (0,width-1) and im1.y in (0,height-1)
    if(0 < im1.x && im1.x < width-1 && 0 < im1.y && im1.y < height-1)
    {
        model3D[curr].z = ((float) plane_index[curr] / (float) n_planes) * 0.1;// * d_min;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

/*__kernel void project_plane (
    __constant float4 * P_inv,      // Inverse of the projection matrix into the image1
    __constant float4 * image1,     // Color field of the image1
    __constant uint * plane_index,  // plane index calculated by the compute_plane kernel
    __global float4 * model3D,      // 3D model
    int height,                     // width of the image1 
    int width)                      // heigth of the image1
{
    __private uint curr = get_global_id(0);

    __private float4 proj;  // final positions of the 3D model

    __private uint2 im1;    // point containning the coordinate of the pixel in image1

    __private float4 m1;    // pixel in image1 in space coordinates

    __private uint i        // index of the plane index (plane_index[curr]) 
 
    // linear indexing to (x,y) matrix indexing for dealing with images
    im1.y = curr / width;
    im1.x = curr - width * im1.y;

    // coordinates must be with range: im1.x in (0,width-1) and im1.y in (0,height-1)
    if(0 < im1.x && im1.x < width-1 && 0 < im1.y && im1.y < height-1)
    {
        m1.x = im1.x;
        m1.y = im1.y;
        m1.z = 1;
        m1.w = 0;

        i = plane_index[curr];

        proj.x = P_inv[ i ] * m1;
        proj.y = P_inv[i+1] * m1;
        proj.z = P_inv[i+2] * m1;

        model3D[curr].z = proj;
    }
}*/

////////////////////////////////////////////////////////////////////////////////////////

/*
 * =====================================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Created:  2014-10-22 17:13 
 *         Author:  Tiago Lobato Gimenes        (tlgimenes@gmail.com)
 * =====================================================================================
 */

#include <opencv2/opencv.hpp>
#include <CL/cl.hpp>
#include <vector>

/* ======= Function ==================================================
 *   Name: main
 *   Description: main entry Function
 * =================================================================== 
 */
int main(int argc, const char **argv)
{
    cv::VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    cv::Mat edges;
    cv::namedWindow("orignal", 1);
    cv::namedWindow("edges",1);
    for(;;)
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera
        cv::cvtColor(frame, edges, CV_BGR2GRAY);
        cv::GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
        cv::Canny(edges, edges, 0, 30, 3);
        cv::imshow("edges", edges);
        cv::imshow("orignal", frame);
        if(cv::waitKey(30) >= 0) {
            std::vector<int> comp_params;
            comp_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
            comp_params.push_back(3);

            cv::imwrite("output.png", edges, comp_params);

            break;
        }
    }
    return 0;
}



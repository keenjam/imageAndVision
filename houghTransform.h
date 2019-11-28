// header inclusion
#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup
#include <math.h>


std::vector<cv::Rect> circleHoughDetector(cv::Mat image);

int lineHoughDetector(cv::Mat image);

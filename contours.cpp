#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "data.h"
#include "contours.h"
#include <iostream>
#include <stdio.h>
#include <vector>


using namespace std;
using namespace cv;

int setupForContours(cv::Mat image) {

  Mat grey_image;
  cvtColor( image, grey_image, CV_BGR2GRAY );

  blur( grey_image, grey_image, Size(3,3));

  Mat canny_output;
  Canny( grey_image, canny_output, 300, 455);

  imwrite( "cannyOutput.jpg", canny_output);

  std::vector<std::vector<Point> > contours;

  findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

  std::vector<RotatedRect> minRect( contours.size() );
  std::vector<RotatedRect> minEllipse( contours.size() );

  int count = 0;

  for( size_t i = 0; i < contours.size(); i++) {
    minRect[i] = minAreaRect( contours[i]);
    if( contours[i].size() > 70 && contours[i].size() < 300  ) {
      minEllipse[i] = fitEllipse(contours[i]);
      count += 1;
    }
  }

  Mat drawing = image.clone();
  for( size_t i = 0; i < contours.size(); i++) {

    // contours
    //drawContours( drawing, contours, (int)i, Scalar( 255, 0,  0), 2);

    // ellipse
    ellipse( drawing, minEllipse[i], Scalar( 0, 255, 0 ), 2);

    // rotated rectangle
    Point2f rect_points[4];
    minRect[i].points( rect_points );
    for( int j = 0; j < 4; j++) {
      //line( drawing, rect_points[j], rect_points[(j+1)%4], Scalar( 255, 255, 0 ));
    }

  }

  imwrite( "contours.jpg", drawing );

  return count;


}

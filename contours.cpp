#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "data.h"
#include "contours.h"
#include "houghTransform.h"
#include <iostream>
#include <stdio.h>
#include <vector>


using namespace std;
using namespace cv;

std::vector<Rect> setupForContours(cv::Mat image) {

  Mat thresholdedImage2 = setup(image);
  Mat thresholdedImage2Norm;
  cv::normalize(thresholdedImage2, thresholdedImage2Norm, 0, 255, NORM_MINMAX, CV_8UC1);

  // Mat grey_image;
  // cvtColor( image, grey_image, CV_BGR2GRAY );
  //
  // blur( grey_image, grey_image, Size(3,3));
  //
  // Mat canny_output;
  // Canny( grey_image, canny_output, 150, 250);
  //
  // imwrite( "cannyOutput.jpg", canny_output);


  std::vector<std::vector<Point> > contours;

  findContours( thresholdedImage2Norm, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

  std::vector<RotatedRect> minRect( contours.size() );
  std::vector<RotatedRect> minEllipse( contours.size() );
  for( size_t i = 0; i < contours.size(); i++) {
    minRect[i] = minAreaRect( contours[i]);
    if( contours[i].size() > 110) {
      minEllipse[i] = fitEllipse(Mat(contours[i]));
    }
  }

  Mat drawing = image.clone();
  Point2f rect_points[4];
  std::vector<Rect> countourBoxes;

  for( size_t i = 0; i < contours.size(); i++) {

    // contours
    drawContours( drawing, contours, (int)i, Scalar( 255, 0,  0), 2);

    // ellipse
    ellipse( drawing, minEllipse[i], Scalar( 0, 255, 0 ), 2);

    // rotated rectangle

    minEllipse[i].points( rect_points );
    for( int j = 0; j < 4; j++) {
      line( drawing, rect_points[j], rect_points[(j+1)%4], Scalar( 255, 255, 0 ));

    }

    Point2f topR(rect_points[0]);
    Point2f bottomR(rect_points[1]);
    Point2f bottomL(rect_points[2]);
    Point2f topL(rect_points[3]);

    if(topR.x != 0 && bottomL.x != 0) {
      int maxX = max(max((int) topR.x, (int) bottomR.x), max((int) bottomL.x, (int) bottomR.x));
      int maxY = max(max((int) topR.y, (int) bottomR.y), max((int) bottomL.y, (int) bottomR.y));

      int minX = min(min((int) topR.x, (int) bottomR.x), min((int) bottomL.x, (int) bottomR.x));
      int minY = min(min((int) topR.y, (int) bottomR.y), min((int) bottomL.y, (int) bottomR.y));

      Point2f centre(((topR.y - bottomR.y) / 2), (topL.x - topR.x) / 2);


      countourBoxes.push_back({minX,minY, maxX-minX, maxY-minY});
    }










  }

  imwrite( "contours.jpg", drawing );

  int count = 0;
  return countourBoxes;


}

/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - face.cpp
//
/////////////////////////////////////////////////////////////////////////////

// header inclusion
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "data.h"
#include "houghTransform.h"
#include <iostream>
#include <stdio.h>
#include <vector>


using namespace std;
using namespace cv;

/** Function Headers */
std::vector<Rect> detectAndDisplay( Mat frame );
std::vector<Rect> detectNoDisplay( Mat frame );
void displayTruths(Mat frame, std::vector<Rect > data);
int getTP(std::vector<Rect > data, std::vector<Rect> detected, Mat frame);
float getIOU(cv::Rect truth, cv::Rect detected);


/** Global variables */
String cascade_name = "frontalface.xml";
CascadeClassifier cascade;
float IOUthresh = 0.45;
std::vector<Rect > truthData;


/** @function main */
int main( int argc, const char** argv )
{

	truthData = getFaceData(argv[1]);
	if(argc > 1 && strcmp(argv[2],"face") != 0) {
		cascade_name = "dartcascade/cascade.xml";
		truthData = getDartData(argv[1]);
	}

	char* imageName;
	strstr(imageName, argv[1]);
  //1. Read Input Image
	Mat frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);


	// 2. Load the Strong Classifier in a structure called `Cascade'
	if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	std::vector<Rect> detected;
	// 3. Detect objects and Display Result
	if(strcmp(argv[2],"hough") != 0) {
		detected = detectAndDisplay( frame );
	}
	else{
		//Detect circles
		std::vector<Rect> circles = circleHoughDetector(frame);

		//Use viola jones and 
		std::vector<Rect> violaDetected = detectNoDisplay(frame);
		std::vector<int> noLines{};

		for (int box = 0; box < violaDetected.size(); box++){
			Mat area;
			float factor = 250.0/frame(violaDetected[box]).rows;

			int xSize = frame(violaDetected[box]).rows * factor;
			int ySize = frame(violaDetected[box]).cols * factor;

			//printf("xFactor: %f, yFactor: %f\n", xFactor,yFactor);
			//printf("x: %d, y: %d\n", xSize,ySize);

			resize(frame(violaDetected[box]),area, Size(xSize,ySize));
			int lines = lineHoughDetector(area);
			//Debug output for viola jones boxes
			noLines.push_back(lines);
			//printf("Detected %d lines\n", lines);
		}

    std::vector<Rect> validViola{};

		for (int box = 0; box < violaDetected.size(); box++){
			if(noLines[box] > 2 && noLines[box] < 20) {
        validViola.push_back(violaDetected[box]);
        //Draw viola estimates
				//rectangle(frame, Point(violaDetected[box].x, violaDetected[box].y), Point(violaDetected[box].x + violaDetected[box].width, violaDetected[box].y + violaDetected[box].height), Scalar( 255, 0, 0 ), 2);
			}
		}

    //Draw circle estimates
    // for (int circle = 0; circle < circles.size(); circle++){
    //   rectangle(frame, Point(circles[circle].x, circles[circle].y), Point(circles[circle].x + circles[circle].width, circles[circle].y + circles[circle].height), Scalar( 255, 255, 0 ), 2);
    // }

    if(circles.size() == 0) {
      detected = validViola;
    }
    else if (validViola.size() == 0) {
      detected = circles;
    }
    else {
      for (int circle = 0; circle < circles.size(); circle++){
        float maxIOU = 0;
        int bestGuess = -1;
        for (int box = 0; box < validViola.size(); box ++) {
          float IOU = getIOU(circles[circle], validViola[box]);
          
          if(IOU > maxIOU) {
            maxIOU = IOU;
            bestGuess = box;
          }
        }

        if(bestGuess != -1){
          int x = min(circles[circle].x, validViola[bestGuess].x);
          int y = min(circles[circle].y, validViola[bestGuess].y);

          int maxX = max(circles[circle].x + circles[circle].width, validViola[bestGuess].x + validViola[bestGuess].width);
          int maxY = max(circles[circle].y + circles[circle].height, validViola[bestGuess].y + validViola[bestGuess].height);


          detected.push_back({x,y,maxX-x, maxY-y});
        }
      }
    }

		for( int i = 0; i < detected.size(); i++ )
		{
			rectangle(frame, Point(detected[i].x, detected[i].y), Point(detected[i].x + detected[i].width, detected[i].y + detected[i].height), Scalar( 0, 255, 0 ), 2);
		}
		
	}

	displayTruths(frame, truthData);

	int count = getTP(truthData, detected, frame);

	float TPR = (float) count/ (float) truthData.size();

	printf("True Positive Rate: %f\n", TPR);

	float precision = (float) count / (float) detected.size();

	float FScore = 2 * (TPR * precision) / (precision + TPR);
	printf("F1 Score: %f\n", FScore);

	// 4. Save Result Image


	imwrite( "detected.jpg", frame );

	return 0;
}


/** @function detectAndDisplay */
std::vector<Rect> detectNoDisplay( Mat frame )
{
	std::vector<Rect> detected;
	Mat frame_gray;

	// 1. Prepare Image by turning it into Grayscale and normalising lighting
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	// 2. Perform Viola-Jones Object Detection
	cascade.detectMultiScale( frame_gray, detected, 1.6, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );

  // 3. Print number of objects found
	std::cout << detected.size() << std::endl;

	return detected;

}
/** @function detectAndDisplay */
std::vector<Rect> detectAndDisplay( Mat frame )
{
	std::vector<Rect> detected;
	Mat frame_gray;

	// 1. Prepare Image by turning it into Grayscale and normalising lighting
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	// 2. Perform Viola-Jones Object Detection
	cascade.detectMultiScale( frame_gray, detected, 1.6, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );

  // 3. Print number of objects found
	std::cout << detected.size() << std::endl;

  // 4. Draw box around objects found
	for( int i = 0; i < detected.size(); i++ )
	{
		rectangle(frame, Point(detected[i].x, detected[i].y), Point(detected[i].x + detected[i].width, detected[i].y + detected[i].height), Scalar( 0, 255, 0 ), 2);
	}

	return detected;

}

void displayTruths(Mat frame, std::vector<Rect > data) {

	for (int truth = 0; truth < data.size(); truth++) {
		rectangle(frame, Point(data[truth].x, data[truth].y), Point(data[truth].x + data[truth].width, data[truth].y + data[truth].height), Scalar( 0, 0, 255 ), 2);
	}
}

int getTP(std::vector<Rect > data, std::vector<Rect> detected, Mat frame) {
	int count = 0;

	for (int truth = 0; truth < data.size(); truth++) {
    int isDet = 0;
	  for (int item = 0; item < detected.size(); item++) {
		  float IOU = getIOU(data[truth], detected[item]);

		  if (IOU >= IOUthresh) {
			  isDet = 1;
		  }
		}
    count += isDet;

	}
	return count;

}

float getIOU(cv::Rect truth, cv::Rect detected) {
  float IOU = 0;
  int interRect[] = {std::max(truth.x,detected.x), std::max(truth.y,detected.y), std::min(truth.x+truth.width,detected.x + detected.width), std::min(truth.y+truth.height,detected.y + detected.height)};

  int interArea = std::max(0, interRect[2] - interRect[0]) * std::max(0, interRect[3] - interRect[1]);
  int truthArea = truth.width * truth.height;
  int detectArea = detected.width * detected.height;

  IOU = std::max(IOU,(float) interArea / (float) (truthArea + detectArea - interArea));

  return (IOU);
}

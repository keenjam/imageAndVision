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
void displayTruths(Mat frame, std::vector<std::vector<int> > data);
int getTP(std::vector<std::vector<int> > data, std::vector<Rect> detected, Mat frame);


/** Global variables */
String cascade_name = "frontalface.xml";
CascadeClassifier cascade;
float IOUthresh = 0.45;
std::vector<std::vector<int> > truthData;


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
		Mat hough = houghTransform(frame);
		printf("hough\n");
		//get hough detection
		//display detected circles
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
std::vector<Rect> detectAndDisplay( Mat frame )
{
	std::vector<Rect> detected;
	Mat frame_gray;

	// 1. Prepare Image by turning it into Grayscale and normalising lighting
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	// 2. Perform Viola-Jones Object Detection
	cascade.detectMultiScale( frame_gray, detected, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );

  // 3. Print number of objects found
	std::cout << detected.size() << std::endl;

  // 4. Draw box around objects found
	for( int i = 0; i < detected.size(); i++ )
	{
		rectangle(frame, Point(detected[i].x, detected[i].y), Point(detected[i].x + detected[i].width, detected[i].y + detected[i].height), Scalar( 0, 255, 0 ), 2);
	}

	return detected;

}

void displayTruths(Mat frame, std::vector<std::vector<int> > data) {

	for (int truth = 0; truth < data.size(); truth++) {
		rectangle(frame, Point(data[truth][0], data[truth][1]), Point(data[truth][0] + data[truth][2], data[truth][1] + data[truth][3]), Scalar( 0, 0, 255 ), 2);
	}
}

int getTP(std::vector<std::vector<int> > data, std::vector<Rect> detected, Mat frame) {
	int count = 0;

	for (int item = 0; item < detected.size(); item++) {
		float IOU = 0;

		for (int truth = 0; truth < data.size(); truth++) {
			int interRect[] = {std::max(data[truth][0],detected[item].x), std::max(data[truth][1],detected[item].y), std::min(data[truth][0]+data[truth][2],detected[item].x + detected[item].width), std::min(data[truth][1]+data[truth][3],detected[item].y + detected[item].height)};

			//draw intersecting area
			//rectangle(frame, Point(interRect[0], interRect[1]), Point(interRect[2], interRect[3]), Scalar( 255, 0, 0 ), 2);

			int interArea = std::max(0, interRect[2] - interRect[0]) * std::max(0, interRect[3] - interRect[1]);
			int truthArea = data[truth][2] * data[truth][3];
			int detectArea = detected[item].width * detected[item].height;

			IOU = std::max(IOU,(float) interArea / (float) (truthArea + detectArea - interArea));
		}

		if (IOU >= IOUthresh) {
			count += 1;
		}
	}
	return count;

}

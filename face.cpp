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
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;
using namespace cv;

/** Function Headers */
std::vector<Rect> detectAndDisplay( Mat frame );
void displayTruths(Mat frame, std::vector<std::vector<int> > data);
int getTP(std::vector<std::vector<int> > data, std::vector<Rect> faces, Mat frame);


/** Global variables */
String cascade_name = "frontalface.xml";
CascadeClassifier cascade;
float IOUthresh = 0.45;


/** @function main */
int main( int argc, const char** argv )
{
       // 1. Read Input Image
	Mat frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);


	std::vector<std::vector<int> > faceData = getData(argv[1]);

	// 2. Load the Strong Classifier in a structure called `Cascade'
	if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	// 3. Detect Faces and Display Result
	std::vector<Rect> faces = detectAndDisplay( frame );

	displayTruths(frame, faceData);

	int count = getTP(faceData, faces, frame);

	float TPR = (float) count/ (float) faceData.size();

	printf("True Positive Rate: %f\n", TPR);

	float precision = (float) count / (float) faces.size();

	float FScore = 2 * (TPR * precision) / (precision + TPR);
	printf("F1 Score: %f\n", FScore);

	// 4. Save Result Image
	imwrite( "detected.jpg", frame );

	return 0;
}

/** @function detectAndDisplay */
std::vector<Rect> detectAndDisplay( Mat frame )
{
	std::vector<Rect> faces;
	Mat frame_gray;

	// 1. Prepare Image by turning it into Grayscale and normalising lighting
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	// 2. Perform Viola-Jones Object Detection 
	cascade.detectMultiScale( frame_gray, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );

       // 3. Print number of Faces found
	std::cout << faces.size() << std::endl;

       // 4. Draw box around faces found
	for( int i = 0; i < faces.size(); i++ )
	{
		rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar( 0, 255, 0 ), 2);
	}

	return faces;

}

void displayTruths(Mat frame, std::vector<std::vector<int> > data) {

	for (int face = 0; face < data.size(); face++) {
		rectangle(frame, Point(data[face][0], data[face][1]), Point(data[face][0] + data[face][2], data[face][1] + data[face][3]), Scalar( 0, 0, 255 ), 2);
	}
}

int getTP(std::vector<std::vector<int> > data, std::vector<Rect> faces, Mat frame) {
	int count = 0;

	for (int detected = 0; detected < faces.size(); detected++) {
		float IOU = 0;

		for (int truth = 0; truth < data.size(); truth++) {
			int interRect[] = {std::max(data[truth][0],faces[detected].x), std::max(data[truth][1],faces[detected].y), std::min(data[truth][0]+data[truth][2],faces[detected].x + faces[detected].width), std::min(data[truth][1]+data[truth][3],faces[detected].y + faces[detected].height)};
			
			//draw intersecting area
			//rectangle(frame, Point(interRect[0], interRect[1]), Point(interRect[2], interRect[3]), Scalar( 255, 0, 0 ), 2);

			int interArea = std::max(0, interRect[2] - interRect[0]) * std::max(0, interRect[3] - interRect[1]);
			int truthArea = data[truth][2] * data[truth][3];
			int detectArea = faces[detected].width * faces[detected].height;

			IOU = std::max(IOU,(float) interArea / (float) (truthArea + detectArea - interArea));
		}

		//printf("Face detected %d IOU: %f\n",detected, IOU);
		if (IOU >= IOUthresh) {
			count += 1;
		}
	}
	return count;

}
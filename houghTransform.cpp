// header inclusion
#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup
#include <math.h>
#include "3darr.h"
#include "houghTransform.h"

using namespace cv;
#define _USE_MATH_DEFINES

void applyKernel(
	cv::Mat &input,
  int kernel[3][3],
	cv::Mat &kernelOutput);

void findGradient(
  cv::Mat &sobelx,
  cv::Mat &sobely,
  cv::Mat &gradientOutput);

void findGradientDirection(
  cv::Mat &sobelx,
  cv::Mat &sobely,
  cv::Mat &gradientDirectionOutput);

std::vector<Rect> houghCircle(
	cv::Mat &thresholdedImage,
	cv::Mat &magnitudeDirectionImage,
  float minimum,
  float maximum,
  int thresh);

std::vector<std::vector<Point> > houghLine(
  cv::Mat &thresholdedImage,
  int thresh);

std::vector<Rect> houghTransform(cv::Mat image)
{

  int thresh = 150;
  int radmin = 35;
  int radmax = 150;
  int houghCircleThresh = 60;
  int houghLineThresh = 55;
 // LOADING THE IMAGE

 //Mat image;
 //image = imread( imageName, 1 );



 // CONVERT COLOUR, BLUR AND SAVE
 Mat gray_image;
 cvtColor( image, gray_image, CV_BGR2GRAY );


 Mat sobelx(gray_image.rows, gray_image.cols, CV_32FC1);
 Mat sobelxnorm(gray_image.rows, gray_image.cols, CV_8UC1);
 int kernelx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
 applyKernel(gray_image, kernelx, sobelx);
 cv::normalize(sobelx, sobelxnorm, 0, 255, NORM_MINMAX, CV_8UC1);
 imwrite( "gen/sobelx.jpg", sobelxnorm );

 Mat sobely(gray_image.rows, gray_image.cols, CV_32FC1);
 Mat sobelynorm(gray_image.rows, gray_image.cols, CV_8UC1);
 int kernely[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
 applyKernel(gray_image, kernely, sobely);
 cv::normalize(sobely, sobelynorm, 0, 255, NORM_MINMAX, CV_8UC1);
 imwrite( "gen/sobely.jpg", sobelynorm );

 Mat magnitudeImage(gray_image.rows, gray_image.cols, CV_32FC1);
 findGradient(sobelx, sobely, magnitudeImage);
 Mat magnitudeImageNorm(gray_image.rows, gray_image.cols, CV_8UC1);
 cv::normalize(magnitudeImage, magnitudeImageNorm, 0, 255, NORM_MINMAX, CV_8UC1);
 imwrite( "gen/magnitudeImage.jpg", magnitudeImageNorm );

 Mat magnitudeDirectionImage(gray_image.rows, gray_image.cols, CV_32FC1);
 findGradientDirection(sobelx, sobely, magnitudeDirectionImage);
Mat magnitudeDirectionImageNorm(gray_image.rows, gray_image.cols, CV_8UC1);
 cv::normalize(magnitudeDirectionImage, magnitudeDirectionImageNorm, 0, 255, NORM_MINMAX, CV_8UC1);
 imwrite( "gen/magnitudeDirectionImage.jpg", magnitudeDirectionImageNorm );

 
 Mat thresholdedImage;
 cv::threshold(magnitudeImage, thresholdedImage, thresh, 255, THRESH_BINARY);
 imwrite( "gen/thresholdedImage.jpg", thresholdedImage );

 std::vector<std::vector<Point > > lines = houghLine(thresholdedImage, houghLineThresh);

 Mat tempImage = image;
 for (int l = 0; l < lines.size(); l++) {
  line(tempImage, lines[l][0], lines[l][1],Scalar( 0, 255, 0 ), 2);
 }

 imwrite("gen/lines.jpg", tempImage);
 return (houghCircle(thresholdedImage, magnitudeDirectionImage, radmin, radmax, houghCircleThresh));

}

void applyKernel(cv::Mat &input, int kernel[3][3], cv::Mat &kernelOutput)
{
  cv::Mat paddedInput;
  cv::copyMakeBorder( input, paddedInput,
    1, 1, 1, 1,
    cv::BORDER_REPLICATE );

	// now we can do the convoltion
	for ( int i = 0; i < input.rows; i++ )
	{
		for( int j = 0; j < input.cols; j++ )
		{
			float sum = 0.0;
			for( int m = -1; m < 2; m++ )
			{
				for( int n = -1; n < 2; n++ )
				{
					// find the correct indices we are using
					int imagex = i + n + 1;
					int imagey = j + m + 1;
					int kernelx = n + 1;
					int kernely = m + 1;

					// get the values from the padded image and the kernel
					float imageval = ( float ) paddedInput.at<uchar>( imagex, imagey );
					float kernelval = kernel[kernely][kernelx];

					// do the multiplication
					sum += imageval * kernelval;
				}
			}
			// set the output value as the sum of the convolution
			kernelOutput.at<float>(i, j) = (float) sum;
		}
	}
}

void findGradient(cv::Mat &sobelx, cv::Mat &sobely, cv::Mat &gradientOutput) {

  for ( int i = 0; i < sobelx.rows; i++ )
	{
		for( int j = 0; j < sobelx.cols; j++ )
		{
      float sobelxValue;
      sobelxValue = sobelx.at<float>(i, j) * sobelx.at<float>(i, j);

      float sobelyValue;
      sobelyValue = sobely.at<float>(i, j) * sobely.at<float>(i, j);

			gradientOutput.at<float>(i, j) = (float) sqrt(sobelxValue + sobelyValue);
		}
	}

}


void findGradientDirection(cv::Mat &sobelx, cv::Mat &sobely, cv::Mat &gradientDirectionOutput) {

  for ( int i = 0; i < sobelx.rows; i++ )
	{
		for( int j = 0; j < sobelx.cols; j++ )
		{
      float sobelxValue;
      sobelxValue = (float) sobelx.at<float>(i, j);

      float sobelyValue;
      sobelyValue = (float) sobely.at<float>(i, j);


			gradientDirectionOutput.at<float>(i, j) = (float) atan2(sobelyValue, sobelxValue) - (M_PI/2);
		}
	}

}

std::vector<Rect> houghCircle(cv::Mat &thresholdedImage, cv::Mat &magnitudeDirectionImage, float minimum, float maximum, int thresh) {
  // radius, y, x
	int ***array = malloc3dArray(maximum, thresholdedImage.rows, thresholdedImage.cols);

	for( int x = 0; x < thresholdedImage.rows; x++) {
		for( int y = 0; y < thresholdedImage.cols; y++ ) {

			for(int r = 0; r < maximum; r++) {
				array[r][x][y] = 0;
			}
		}
	}
	for( int x = 0; x < thresholdedImage.rows; x++) {

		for( int y = 0; y < thresholdedImage.cols; y++ ) {

			if(thresholdedImage.at<float>( x, y ) == 255) {

				for(int r = minimum; r < maximum; r++) {
					int arrayx = (int)(x+(r* sin(magnitudeDirectionImage.at<float>( x, y ))));
					int arrayy = (int)(y+(r* cos(magnitudeDirectionImage.at<float>( x, y ))));
					if(arrayx >=0 && arrayx < thresholdedImage.rows && arrayy >=0 && arrayy < thresholdedImage.cols) {
						array[r][arrayx][arrayy] += 1;
					}

          arrayx = (int)(x-(r* sin(magnitudeDirectionImage.at<float>( x, y ))));
          arrayy = (int)(y-(r* cos(magnitudeDirectionImage.at<float>( x, y ))));
          if(arrayx >=0 && arrayx < thresholdedImage.rows && arrayy >=0 && arrayy < thresholdedImage.cols) {
            array[r][arrayx][arrayy] += 1;
          }
				}
			}

		}
	}
	Mat houghCircleOutput(thresholdedImage.rows, thresholdedImage.cols, CV_32FC1);

  std::vector<Rect > outputRect;
  std::vector<int > vals;

	for( int x = 1; x < thresholdedImage.rows-1; x++) {
		for( int y = 1; y < thresholdedImage.cols-1; y++ ) {
			int sum = 0;
			for (int r = minimum; r < maximum; r++) {
        int val = array[r][x-1][y-1] +  array[r][x][y-1] + array[r][x+1][y-1] + array[r][x-1][y] + array[r][x][y] + array[r][x+1][y] + array[r][x-1][y+1] + array[r][x][y+1] + array[r][x+1][y+1];
        if(val> thresh) {
          if(outputRect.size() == 0){
            outputRect.push_back({y-r,x-r, r * 2, r*2});
            vals.push_back(val);
          }
          else {
            int similar = 0;
            for (int rect = 0; rect < outputRect.size(); rect ++) {
              if (std::abs(y-r - outputRect[rect].x) < 100) {
                similar = 1;
                if(val > vals[rect]) {
                  vals[rect] = val;
                  outputRect[rect] = {y-r,x-r, r * 2, r*2};
                }
              }
            }
            if(!similar){
                outputRect.push_back({y-r,x-r, r * 2, r*2});
                vals.push_back(val);
            }
          }

        }
				//For drawing 2D hough interpretation
        sum += array[r][x][y];

			}
			houghCircleOutput.at<float>(x,y) = (float) sum;

		}
	}


   Mat houghCircleNorm(houghCircleOutput.rows, houghCircleOutput.cols, CV_8UC1);
   cv::normalize(houghCircleOutput, houghCircleNorm, 0, 255, NORM_MINMAX, CV_8UC1);
	 imwrite( "gen/houghCircleOutput.jpg", houghCircleNorm );

   return(outputRect);
}

std::vector<std::vector<Point > > houghLine(cv::Mat &thresholdedImage, int thresh) {

  std::vector<std::vector<Point > > lines;

  int width = 500;
  int height = 500;
  int maxP = (thresholdedImage.rows + thresholdedImage.cols) * 2;

  Mat houghLineImage( height, width, CV_32FC1);

  for( int x = 0; x < thresholdedImage.rows; x++) {
    for( int y = 0; y < thresholdedImage.cols; y++ ) {
      if(thresholdedImage.at<float>( x, y ) == 255) {
        for(int i = 0; i < width; i ++) {
          float theta = ((float) i / (float) width) * M_PI;
          int p = ((x * cos(theta) + y * sin(theta))/maxP) * height + (height/2);
          houghLineImage.at<float>(p,i) += 1;
        }
      }
    }
  }

  for (int x = 0; x < height; x++) {
    for (int y = 0; y < width; y++) {
      if (houghLineImage.at<float>(x,y) > thresh) {
        printf("found a line, val: %f\n", houghLineImage.at<float>(x,y));
        float theta = ((float) y / (float) width) * M_PI;
        float a = cos(theta);
        float b = sin(theta);

        float x0 = a * theta;
        float y0 = b * theta;
        int x1 = (int) (x0 + 1000*(-b));
        int y1 = (int) (y0 + 1000*(a));
        int x2 = (int) (x0 - 1000*(-b));
        int y2 = (int) (y0 - 1000*(a));
        lines.push_back({Point{x1,y1},Point{x2,y2}});
      }
    }
  }

  Mat houghLineNorm(houghLineImage.rows, houghLineImage.cols, CV_8UC1);
  cv::normalize(houghLineImage, houghLineNorm, 0, 255, NORM_MINMAX, CV_8UC1);
  imwrite( "gen/houghLineOutput.jpg", houghLineNorm );

  return lines;
}

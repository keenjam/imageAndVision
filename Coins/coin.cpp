// header inclusion
#include <stdio.h>
#include <opencv/cv.h>        //you may need to
#include <opencv/highgui.h>   //adjust import locations
#include <opencv/cxcore.h>    //depending on your machine setup
#include <math.h>
#include "3darr.h"

using namespace cv;

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

void hough(
	cv::Mat &thresholdedImage,
	cv::Mat &magnitudeDirectionImage,
	int threshold,
  float minimum,
  float maximum,
	float distance);

int main( int argc, char** argv )
{

 // LOADING THE IMAGE
 char* imageName = argv[1];

 Mat image;
 image = imread( imageName, 1 );



 // CONVERT COLOUR, BLUR AND SAVE
 Mat gray_image;
 cvtColor( image, gray_image, CV_BGR2GRAY );


 Mat sobelx(gray_image.rows, gray_image.cols, CV_32FC1);
 Mat sobelxnorm(gray_image.rows, gray_image.cols, CV_8UC1);
 int kernelx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
 applyKernel(gray_image, kernelx, sobelx);
 cv::normalize(sobelx, sobelxnorm, 0, 255, NORM_MINMAX, CV_8UC1);
 imwrite( "sobelx.jpg", sobelxnorm );

 Mat sobely(gray_image.rows, gray_image.cols, CV_32FC1);
 Mat sobelynorm(gray_image.rows, gray_image.cols, CV_8UC1);
 int kernely[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
 applyKernel(gray_image, kernely, sobely);
 cv::normalize(sobely, sobelynorm, 0, 255, NORM_MINMAX, CV_8UC1);
 imwrite( "sobely.jpg", sobelynorm );

 Mat magnitudeImage(gray_image.rows, gray_image.cols, CV_32FC1);
 findGradient(sobelx, sobely, magnitudeImage);
 cv::normalize(magnitudeImage, magnitudeImage, 0, 255, NORM_MINMAX, CV_8UC1);
 imwrite( "magnitudeImage.jpg", magnitudeImage );

 Mat magnitudeDirectionImage(gray_image.rows, gray_image.cols, CV_32FC1);
 findGradientDirection(sobelx, sobely, magnitudeDirectionImage);
 cv::normalize(magnitudeDirectionImage, magnitudeDirectionImage, 0, 255, NORM_MINMAX, CV_8UC1);
 imwrite( "magnitudeDirectionImage.jpg", magnitudeDirectionImage );

 int thresh = 100;
 Mat thresholdedImage;
 cv::threshold(magnitudeImage, thresholdedImage, thresh, 255, THRESH_BINARY);
 imwrite( "thresholdedImage.jpg", thresholdedImage );


 hough(thresholdedImage, magnitudeDirectionImage, 0, 10, 50, 0);

 return 0;

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

void hough(cv::Mat &thresholdedImage, cv::Mat &magnitudeDirectionImage, int threshold, float minimum, float maximum, float distance) {
  // radius, y, x
	int ***array = malloc3dArray(maximum, thresholdedImage.rows, thresholdedImage.cols);

	for( int x = 0; x < thresholdedImage.rows; x++) {
		for( int y = 0; y < thresholdedImage.cols; y++ ) {
			for(int r = minimum; r < maximum; r++) {
				array[r][x][y] = 0;
			}
		}
	}
	for( int x = 0; x < thresholdedImage.rows; x++) {

		for( int y = 0; y < thresholdedImage.cols; y++ ) {

			if(thresholdedImage.at<uchar>( x, y ) == 255) {
				for(int r = minimum; r < maximum; r++) {
					int arrayx = (int)(x+(r* cos(magnitudeDirectionImage.at<uchar>( x, y ))));
					int arrayy = (int)(y+(r* sin(magnitudeDirectionImage.at<uchar>( x, y ))));
					if(arrayx >=0 && arrayx < thresholdedImage.rows && arrayy >=0 && arrayy < thresholdedImage.cols) {
						array[r][arrayx][arrayy] += 1;
					}
				}
			}

		}
	}
	Mat houghOutput(thresholdedImage.rows, thresholdedImage.cols, CV_8UC1);
	for( int x = 0; x < thresholdedImage.rows; x++) {
		for( int y = 0; y < thresholdedImage.cols; y++ ) {
			int sum = 0;
			for (int r = minimum; r < maximum; r++) {
				sum += array[r][x][y];
				houghOutput.at<uchar>(x,y) += (uchar) sum;
			}


		}
	}



	 imwrite( "houghOuput.jpg", houghOutput );
}

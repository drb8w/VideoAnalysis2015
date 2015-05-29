#include "FrameHelpers.h"

#include "stdafx.h"
//#include <vector>
#include <opencv2\opencv.hpp> 

using namespace cv;
using namespace std;

void convertToGrayscale(const Mat &img, Mat &imgGray)
{
	for(int rowNr = 0; rowNr < img.rows; rowNr++ )
	{
		for(int columnNr = 0; columnNr < img.cols; columnNr++ )
		{
			int y = rowNr; // see OpenCV userguide
			int x = columnNr;

			Vec3b intensity = img.at<Vec3b>(y, x);
			uchar blue = intensity.val[0];
			uchar green = intensity.val[1];
			uchar red = intensity.val[2];
			// formula from script
			uchar luminance = 0.21*red + 0.72*green + 0.07*blue;
			imgGray.at<uchar>(y, x) = luminance;
		}
	}
}

vector<Mat> *convertToGrayscales(vector<Mat> *rgbFrames, vector<Mat> *grayFrames)
{
	for(int i=0; i < rgbFrames->size(); i++)
	{
		Mat imgRGB_i = (*rgbFrames)[i];
		Mat imgGray_i(imgRGB_i.rows,imgRGB_i.cols, CV_8U);
		convertToGrayscale(imgRGB_i, imgGray_i);
		grayFrames->push_back(imgGray_i);
	}

	return grayFrames;
}

vector<Mat> *convertToGrayscales(vector<Mat> *rgbFrames)
{
	vector<Mat> *grayFrames = new vector<Mat>();
	return convertToGrayscales(rgbFrames, grayFrames);
}


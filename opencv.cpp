#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include <cmath>
#include <math.h>
#define PI 3.14159265

using namespace std;

cv::Vec3b HSVtoRGB(float H, float S, float V) {
	cv::Vec3b ret2; // the vector for the colored version 

	V = (V / 255) * 100;

	if (H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0) {  // if the params dont have the specified condition it wont work
		cout << "The givem HSV values are not in valid range" << endl;  
		cout << "H = " << H << " S= " << S << " V= " << V << endl;
		ret2[0] = 0;
		ret2[1] = 0;
		ret2[2] = 0;
		return ret2;
	}
	float s = S / 100;   // compute the params for the HSV
	float v = V / 100;
	float C = s * v;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;
	if (H >= 0 && H < 60) {   // fill the rgb with the specified values for specified angle
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else if (H >= 300 && H < 360) {
		r = C, g = 0, b = X;
	}
	ret2[0] = (r + m) * 255;
	ret2[1] = (g + m) * 255;
	ret2[2] = (b + m) * 255;

	return ret2;
}



cv::Mat BGR2GRAY(cv::Mat src) { //Convert to grayscale image
	int rows = src.rows;
	int cols = src.cols;

	cv::Mat dst = cv::Mat::zeros(rows, cols, CV_8UC1);   // create a empty matrix with the dimensions of the file

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			dst.at<uchar>(i, j) = ((float)src.at<cv::Vec3b>(i, j)[2]
		    	+ (float)src.at<cv::Vec3b>(i, j)[1]
				+ (float)src.at<cv::Vec3b>(i, j)[0])/3;                    // we make the avg of the pixels for the grayscale
		}
	}
	return dst;
}
void Sobelfilter(cv::Mat& src) {//sobel filtering
	int rows = src.rows;
	int cols = src.cols;


	double dx = 0, dy = 0;
	double dx2 = 0, dy2 = 0;
	cv::Mat G = cv::Mat::zeros(rows, cols, CV_8UC1);     //empty matrix for the 
	cv::Mat G2 = cv::Mat::zeros(rows, cols, CV_8UC3);
	double v = 0, vx, vy;
	double r = 0, g = 0, b = 0;
	double arc = 0;
	double angle;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			v = 0;
			if (i == 0 || j == 0 || i == rows - 1 || j == cols - 1) {
				G.at<uchar>(i, j) = 0;
			}
			else {
				dx = src.at<uchar>(i - 1, j + 1) - src.at<uchar>(i - 1, j - 1)    // calculate the gradient
					+ 2 * src.at<uchar>(i, j + 1) - 2 * src.at<uchar>(i, j - 1) +
					src.at<uchar>(i + 1, j + 1) - src.at<uchar>(i + 1, j - 1);
				dy = src.at<uchar>(i + 1, j - 1) - src.at<uchar>(i - 1, j - 1)
					+ 2 * src.at<uchar>(i + 1, j) - 2 * src.at<uchar>(i - 1, j) +
					src.at<uchar>(i + 1, j + 1) - src.at<uchar>(i - 1, j + 1);

				dx2 = dx;
				dy2 = dy;

				if (dx > 0 && dy > 0) {
					angle = atan(dy /dx - PI) * 540 / PI;   // calculate the angle
					angle = abs(angle);
					//angle = 360 - angle;  <- for inverting the color
				}else if(dx < 0 && dy > 0) {
					angle = (atan(dy / dx)) * 720 / PI;   // calculate the angle
					angle = abs(angle);
					//angle = 360 - angle;
				}else if (dx < 0 && dy < 0) {
					angle = (atan((dy) / (dx))+PI) * 180 / PI;   // calculate the angle
					angle = abs(angle);
					//angle = 360 - angle;
				}
				else {
					angle = atan((dy) / (dx)) * 360 / PI;   // calculate the angle
					angle = abs(angle);
					//angle = 360 - angle;
				}

				//cout << angle << endl;

				v = abs(dx) + abs(dy); 


				v = fmax(v, 0);				
				v = fmin(v, 255);


				G2.at<cv::Vec3b>(i, j) = HSVtoRGB(angle, 100, v);
				G.at<uchar>(i, j) = (uchar)v;

				

				




			}
		}
	}
	
	cv::imshow("G", G);
	cv::imshow("G2", G2);
}


int main(int argc, char** argv) {
	cv::Mat src, gray;
	src = cv::imread("C:/Users/georg/OneDrive/Desktop/circle.jpg");  //read the image into the Mat src

	gray = BGR2GRAY(src);  //convert the image to grayscale

	Sobelfilter(gray); // apply the filter
	cv::imshow("G1", src);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}
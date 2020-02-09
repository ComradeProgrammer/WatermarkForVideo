#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<vector>
class WatermarkExtractor {
public:
	WatermarkExtractor(cv::VideoCapture _cap);
	void proceedVideo();
	cv::Mat extractWatermark(cv::Mat frame,cv::Mat newFrame);
	void setProperties(int _extractorFactor, int _energy,int _verticalBias,
		int _horizontalBias, int _insertInterval);
private:
	cv::VideoCapture cap;
	double width;
	double height;
	double fps;
	int frameCount;
	int energy;
	int extractorFactor;
	int verticalBias;
	int horizontalBias;
	int insertInterval;
};
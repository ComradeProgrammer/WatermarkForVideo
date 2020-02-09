#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<string>
#include<vector>
class WatermarkAdder {
public:
	WatermarkAdder(cv::VideoCapture _cap);
	void proceedVideo(std::string outputFileName);
	cv::Mat proceedFrame(cv::Mat frame,int formation);
	cv::Mat getInformationGraph(int information, int energy);
	void setProperties(int _energy, int _verticalBias, int _horizontalBias,
		int _insertInterval);
private:
	cv::VideoCapture cap;
	cv::VideoWriter outputVideo;
	double width;
	double height;
	double fps;
	int frameCount;

	int energy;
	int verticalBias;
	int horizontalBias;
	int insertInterval;


	
};
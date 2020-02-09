#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
class Picture {
public:
	Picture();
	Picture(cv::Mat _m);
	std::vector<cv::Mat> pictureDct();
	void pictureIdct(std::vector<cv::Mat> input);

	void show();

	cv::Mat img;
	std::vector<cv::Mat>channels;
};
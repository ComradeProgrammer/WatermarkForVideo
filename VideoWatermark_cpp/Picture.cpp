#include"Picture.h"
using namespace cv;

Picture::Picture(Mat _m) {
	img = _m.clone();
	split(img, channels);
}

Picture::Picture() {

}

std::vector<Mat> Picture::pictureDct() {
	//extract different channels
	Mat blueChannel = channels.at(0);
	Mat greenChannel = channels.at(1);
	Mat redChannel = channels.at(2);

	/*convert to float matrices*/
	Mat blueTmp,greenTmp,redTmp;
	blueChannel.convertTo(blueTmp, CV_32F, 1.0 );
	greenChannel.convertTo(greenTmp, CV_32F, 1.0 );
	redChannel.convertTo(redTmp, CV_32F, 1.0 );
	//blueChannel.convertTo(blueTmp, CV_32F, 1.0 / 255);
	//greenChannel.convertTo(greenTmp, CV_32F, 1.0 / 255);
	//redChannel.convertTo(redTmp, CV_32F, 1.0 / 255);

	/*dct*/
	Mat blueResult, greenResult, redResult;
	dct(blueTmp, blueResult);
	dct(greenTmp, greenResult);
	dct(redTmp, redResult);

	/*return value*/
	std::vector<Mat> res;
	res.push_back(blueResult);
	res.push_back(greenResult);
	res.push_back(redResult);
	return res;
	
}

void Picture::pictureIdct(std::vector<Mat> input) {
	Mat blueInput = input[0];
	Mat greenInput = input[1];
	Mat redInput = input[2];
	//Mat blueRev=input[0],greenRev=input[1],redRev=input[2];
	Mat blueRev(512, 512, CV_32F);
	Mat greenRev(512, 512, CV_32F);
	Mat redRev(512, 512, CV_32F);
	
	dct(blueInput, blueRev, DCT_INVERSE);
	dct(greenInput, greenRev, DCT_INVERSE);
	dct(redInput, redRev, DCT_INVERSE);

	int rows = blueRev.rows;
	int cols = blueRev.rows;
	
	blueRev.convertTo(blueRev, CV_8UC1, 1);
	greenRev.convertTo(greenRev, CV_8UC1, 1);
	redRev.convertTo(redRev, CV_8UC1, 1);

	channels.clear();
	channels.push_back(blueRev);
	channels.push_back(greenRev);
	channels.push_back(redRev);

	merge(channels, img);
}

void Picture::show() {
	imshow("", img);
	waitKey(0);
}
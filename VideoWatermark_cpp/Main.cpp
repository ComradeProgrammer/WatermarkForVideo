#include"main.h"
using namespace cv;
int main()
{
	/*
	Mat img = imread("t.jpg");
	Picture p(img);
	std::vector<Mat> res=p.pictureDct();

	Picture p2;
	p2.pictureIdct(res);
	p2.show();
	*/	
	
	/*VideoCapture cap("testvideo2.mp4");
	WatermarkAdder t(cap);
	t.setProperties(255, 32,32);
	Mat img = imread("xin.jpg");
	Mat tmp = t.proceedFrame(img, 555);
	imshow("", tmp);
	waitKey(0);*/
	/*t.proceedVideo("test.mp4");*/

	/*
	VideoCapture cap("testvideo2.mp4");
	WatermarkAdder t(cap);
	t.setProperties(20, 32, 32,27);
	t.proceedVideo("test.mp4");
	*/
	/*
	VideoCapture cap("testvideo2.mp4");
	WatermarkAdder t(cap);
	t.setProperties(20, 32, 32,27);
	Mat img = imread("xin.jpg");
	Mat tmp = t.proceedFrame(img, 555);
	imwrite("fuck.jpg", tmp);
	Mat tmp2 = imread("fuck.jpg");
	WatermarkExtractor t2(cap);
	t2.setProperties(12,20, 32, 32, 27);
	Mat gg = t2.extractWatermark(img, tmp2);
	imshow("", gg);
	waitKey(0);
	*/
	VideoCapture cap("testvideo2.mp4");
	WatermarkAdder t(cap);
	t.setProperties(20, 320-64, 320 - 64, 5);
	t.proceedVideo("test.mp4");
	VideoCapture cap2("test.mp4"); 
	WatermarkExtractor t2(cap2);
	t2.setProperties(10, 20, 320 - 64, 320 - 64, 5);
	t2.proceedVideo();
}
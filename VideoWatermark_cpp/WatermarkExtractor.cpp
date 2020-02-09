#include"WatermarkExtractor.h"
#include"Picture.h"
using namespace cv;
using std::cout;
using std::endl;
using std::vector;

WatermarkExtractor::WatermarkExtractor(cv::VideoCapture _cap) {
	cap = _cap;
	width = cap.get(CAP_PROP_FRAME_WIDTH);
	height = cap.get(CAP_PROP_FRAME_HEIGHT);
	fps = cap.get(CAP_PROP_FPS);
	frameCount = cap.get(CAP_PROP_FRAME_COUNT);
	cout << "width=" << width << endl;
	cout << "height=" << height << endl;
	cout << "fps=" << fps << endl;
	cout << "frame count=" << frameCount << endl;
}

void WatermarkExtractor::setProperties(int _extractorFactor, int _energy, int _verticalBias,
	int _horizontalBias, int _insertInterval) {
	extractorFactor = _extractorFactor;
	verticalBias = _verticalBias;
	energy = _energy;
	horizontalBias = _horizontalBias;
	insertInterval = _insertInterval+1;
}

void WatermarkExtractor::proceedVideo() {
	for (int i = 0; i < frameCount;i++) {
		Mat frame;
		cap.read(frame);
		if (i % insertInterval == 0) {
			i++;
			Mat frame2;
			cap.read(frame2);
			Mat res=extractWatermark(frame, frame2);
			imshow("", res);
			waitKey(0);
		}
	}
}

Mat  WatermarkExtractor::extractWatermark(cv::Mat frame, cv::Mat newFrame) {
	Picture pic(frame);
	Picture newPic(newFrame);

	vector<Mat> dct = pic.pictureDct();
	vector<Mat> newDct = newPic.pictureDct();

	Mat blue(64, 64, CV_8UC1);
	Mat green(64, 64, CV_8UC1);
	Mat red(64, 64, CV_8UC1);

	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int tmp0=(int)(newDct[0].at<float>(i + verticalBias, j + horizontalBias) -
					dct[0].at<float>(i + verticalBias, j + horizontalBias));
			tmp0 = tmp0 > energy / 2 ? energy : 0;
			tmp0 *= extractorFactor;
			blue.at<uchar>(i, j) = tmp0 > 255 ? 255 :
				tmp0 < 0 ? 0 : tmp0;

			int tmp1 = (int)(newDct[1].at<float>(i + verticalBias, j + horizontalBias) -
				dct[1].at<float>(i + verticalBias, j + horizontalBias));
			tmp1 = tmp1 > energy / 2 ? energy : 0;
			tmp1 *= extractorFactor;
			green.at<uchar>(i, j) = tmp1 > 255 ? 255 :
				tmp1 < 0 ? 0 : tmp1;

			int tmp2 = (int)(newDct[2].at<float>(i + verticalBias, j + horizontalBias) -
				dct[2].at<float>(i + verticalBias, j + horizontalBias));
			tmp2 = tmp0 > energy / 2 ? energy : 0;
			tmp2 *= extractorFactor;
			red.at<uchar>(i, j) = tmp2 > 255 ? 255 :
				tmp2 < 0 ? 0 : tmp2;
		}
	}
	vector<Mat> channels;
	channels.push_back(blue);
	channels.push_back(green);
	channels.push_back(red);
	Mat res;
	merge(channels, res);
	return res;

}
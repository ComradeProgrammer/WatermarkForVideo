#include"WatermarkAdder.h"
#include"Picture.h"
using namespace cv;
using std::endl;
using std::cout;
using std::string;
using std::vector;
WatermarkAdder::WatermarkAdder(cv::VideoCapture _cap) {
	cap = _cap;
	/*basic information of the input video*/
	width = cap.get(CAP_PROP_FRAME_WIDTH);
	height = cap.get(CAP_PROP_FRAME_HEIGHT);
	fps = cap.get(CAP_PROP_FPS);
	frameCount = cap.get(CAP_PROP_FRAME_COUNT);
	cout << "width=" << width << endl;
	cout << "height=" << height << endl;
	cout << "fps=" << fps << endl;
	cout << "frame count=" << frameCount << endl;
}

void WatermarkAdder::setProperties(int _energy, int _verticalBias, 
	int _horizontalBias,int _insertInterval) {
	energy = _energy;
	verticalBias = _verticalBias;
	horizontalBias = _horizontalBias;
	insertInterval = _insertInterval;
}
void WatermarkAdder::proceedVideo(string outputFileName) {
	/*prepare write into new video*/
	VideoWriter videoWriter(outputFileName,
		VideoWriter::fourcc('M','P','4','V'),
		fps/insertInterval*(insertInterval+1),
		Size(width, height)
		);

	for (int i = 0; i < frameCount; i++) {
		Mat frame;
		cap.read(frame);
		videoWriter.write(frame);
		if (i % insertInterval == 0) {
			cout << "proceeding frame " << i << endl;
			Mat newFrame = proceedFrame(frame, i);
			videoWriter.write(newFrame);
		}
	}
}

Mat WatermarkAdder::proceedFrame(Mat frame,int information) {

	Picture pic(frame);
	Mat informationGraph = getInformationGraph(information, energy);
	vector<Mat>dctResult = pic.pictureDct();
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			dctResult[0].at<float>(i + verticalBias, j + horizontalBias) +=
				informationGraph.at<uchar>(i, j);

			dctResult[1].at<float>(i + verticalBias, j + horizontalBias) +=
				informationGraph.at<uchar>(i, j);

			dctResult[2].at<float>(i + verticalBias, j + horizontalBias) +=
				informationGraph.at<uchar>(i, j);
		}
	}
	Picture res;
	res.pictureIdct(dctResult);
	return res.img;
}

Mat WatermarkAdder:: getInformationGraph(int information, int energy) {
	Mat res(64,64, CV_8UC1);
	int pos = 31;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 4; j++) {
			int bit = ((1 << pos) & information);
			for (int k = 0; k < 8; k++) {
				for (int l = 0; l < 16; l++) {
					res.at<uchar>(i * 8 + k,j * 16 + l) = (bit == 0 ? 0 : (uchar)energy);
				}
			}
			pos--;
		}
	}
	return res;
}


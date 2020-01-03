#include "Util.h"

using namespace cv;

#define BRUSH_SIZE Size(136,184)


Util::Util()
{
	//单通道灰度图
	Mat tmpBrush = imread("brush/brush.png", IMREAD_UNCHANGED);
	resize(tmpBrush, tmpBrush, BRUSH_SIZE);

	if (tmpBrush.channels() == 4)
		cvtColor(tmpBrush, tmpBrush, COLOR_BGRA2GRAY);
	else if(tmpBrush.channels() == 3)
		cvtColor(tmpBrush, tmpBrush, COLOR_BGR2GRAY);

	brush = tmpBrush;
	//brush = 255 - brush;
}


Util::~Util()
{
}

void Util::init()
{

}

////print error and execute the function "fp"
//void Util::getError(Status status,string msg,void (*fp)())
//{
//	if (status != STATUS_OK)
//	{
//		cout << msg << endl;
//		fp();
//	}
//}
//
//void Util::getError(Status status, string msg, VideoStream & stream)
//{
//	if (status != STATUS_OK)
//	{
//		cout << msg << endl;
//		stream.destroy();
//	}
//}
//
//void Util::getError(Status status, string msg)
//{
//	if (status != STATUS_OK)
//	{
//		cout << msg << endl;
//	}
//}
//
//void Util::oni2Mat(VideoFrameRef &oniCv,Mat &dst)
//{
//	if (oniCv.getSensorType() == SENSOR_COLOR)
//	{
//		RGB888Pixel *dData = (RGB888Pixel*)oniCv.getData();
//		dst = Mat(oniCv.getHeight(), oniCv.getWidth(), CV_8UC3, dData);
//		cvtColor(dst, dst, COLOR_RGB2BGR);
//	}
//	else if (oniCv.getSensorType() == SENSOR_DEPTH)
//	{
//		DepthPixel *dData = (DepthPixel*)oniCv.getData();
//		dst = Mat(oniCv.getHeight(), oniCv.getWidth(), CV_16UC1, dData);
//	}
//	else
//	{
//		throw runtime_error("Unsupported sensor type.");
//	}
//}

//均衡直方图同时转为每通道8位的图像
void Util::equalizeHist(Mat & src, Mat & dst)
{
	Point minLoc, maxLoc;
	unsigned short min, max;

	minMaxLoc(src, NULL, NULL, &minLoc, &maxLoc);
	min = src.at<unsigned short>(minLoc);
	max = src.at<unsigned short>(maxLoc);

	double scale = 255.0 / (max - min);

	convertScaleAbs(src, dst, scale);
}

//src1为三通道，src2为单通道，两者尺寸要一致
void Util::addWeighted(Mat& src1, Mat& src2, Mat& dst)
{
	int rows = src1.rows;
	int cols = src1.cols;

	for (size_t i = 0; i < rows; i++)
	{
		const uchar* inData1 = src1.ptr<uchar>(i);
		const float16_t* inData2 = src2.ptr<float16_t>(i);
		uchar* outData = dst.ptr<uchar>(i);

		for (size_t j = 0; j < cols; j++)
		{
			//src1三个通道都和src2的单通道相乘
			*outData++ = *inData1++ * *inData2;
			*outData++ = *inData1++ * *inData2;
			*outData++ = *inData1++ * *inData2;
			++inData2;
		}
	}
}

//将src2覆盖到src1上
//正常的透明图层混合模式
void Util::mix(Mat& src1, Mat& src2, Mat& dst)
{
	vector<Mat> src1Channels;
	vector<Mat> src2Channels;

	split(src1, src1Channels);
	split(src2, src2Channels);

	Mat invAlpha = 255 - src2Channels[3];

	for (int i = 0; i < 3; i++)
	{
		src1Channels[i] = src1Channels[i].mul(invAlpha, 1.0 / 255);
		src1Channels[i] += src2Channels[i].mul(src2Channels[3], 1.0 / 255);
	}

	merge(src1Channels, dst);
}

//x,y为中心坐标
void Util::erase(Mat& src, Mat& dst,int x,int y)
{
	int lefttopX = 0;
	int lefttopY = 0;

	int brushX = 0;
	int brushY = 0;
	int brushWidth = brush.cols;
	int brushHeight = brush.rows;

	//左边界
	if (x > brush.cols / 2)
		lefttopX = x - brush.cols / 2;
	else {
		brushX = brush.cols / 2 - x;
		brushWidth = brush.cols - brushX;
	}

	//右边界
	if (x >= src.cols - brush.cols / 2)
		brushWidth = src.cols - lefttopX;

	//上边界
	if (y > brush.rows / 2)
		lefttopY = y - brush.rows / 2;
	else {
		brushY = brush.rows / 2 - y;
		brushHeight = brush.rows - brushY;
	}

	//下边界
	if (y >= src.rows - brush.rows / 2)
		brushHeight = src.rows - lefttopY;

	Rect srcROI(lefttopX, lefttopY, brushWidth, brushHeight);
	Rect brushROI(brushX, brushY, brushWidth, brushHeight);

	vector<Mat> srcChannels;
	split(src, srcChannels);

	//3个通道
	//for (int i = 0; i < 3; i++)
	//{
	//	srcChannels[i](srcROI) = srcChannels[i](srcROI).mul(brush(brushROI), 1.0 / 255);
	//}

	//alpha通道
	//srcChannels[3](srcROI) = srcChannels[3](srcROI).mul(brush(brushROI), 1.0 / 255);

	srcChannels[3](srcROI) -= brush(brushROI);

	merge(srcChannels, dst);
}

Mat Util::brush;

Util useless;




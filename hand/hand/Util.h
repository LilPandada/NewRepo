#pragma once

#include<iostream>
#include<string>

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>




using namespace std;



class Util
{
public:
	Util();
	~Util();

	static void init();
	//static void getError(Status status,string msg, void(*fp)());
	//static void getError(Status status, string msg, VideoStream &stream);
	//static void getError(Status status, string msg);
	//static void oni2Mat(VideoFrameRef &oniCv, Mat &dst);
	static void equalizeHist(cv::Mat &src, cv::Mat &dst);
	static void addWeighted(cv::Mat& src1, cv::Mat& src2, cv::Mat& dst);
	static void mix(cv::Mat& src1, cv::Mat& src2, cv::Mat& dst);
	static void erase(cv::Mat& src, cv::Mat& dst, int x, int y);

	static cv::Mat brush;					//单通道灰度图
};


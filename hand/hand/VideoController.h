#pragma once

#include<iostream>
#include<string>
#include<vector>

#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<Windows.h>
#include<windows.h>



#pragma comment(lib,"winmm.lib")

using namespace std;



#define ANIMATION_NUM 1

const cv::Size VIDEO_SIZE = cv::Size(640, 480);

typedef enum {
	before,
	playing,
	after
}State;


//class VideoPlayer
//{
//public:
//	VideoPlayer() {}
//	VideoPlayer(cv::String videoName)
//	{
//		mVideoCapture = cv::VideoCapture(videoName);
//		if (!mVideoCapture.isOpened())
//		{
//			cout << "open video failed" << endl;
//		}
//
//		mVideoCapture.set(cv::CAP_PROP_POS_FRAMES, 10);
//	}
//
//	VideoPlayer(cv::Point point, cv::String videoName) : mPoint(point)
//	{
//		mVideoCapture = cv::VideoCapture(videoName);
//		if (!mVideoCapture.isOpened())
//		{
//			cout << "open video failed" << endl;
//		}
//	}
//
//	void getFrame()
//	{
//		if (!isPlaying)
//			return;
//
//		if (!mVideoCapture.read(frame))
//		{
//			VideoController::getVideoController()->finishVideo();
//			isPlaying = false;
//			mVideoCapture.set(cv::CAP_PROP_POS_FRAMES, 10);
//			return;
//		}
//
//		cv::resize(frame, frame, VIDEO_SIZE);
//	}
//
//	cv::VideoCapture mVideoCapture;
//	bool isPlaying = false;
//	cv::Point mPoint;
//	cv::Mat frame;
//
//private:
//
//};


//单例
//负责给出所有视频叠加后的图像
class VideoController
{
public:
	VideoController();
	~VideoController();

	static VideoController* getVideoController();

	void play();
	void setXY(int x, int y);
	void finishVideo();

	cv::Mat mDepthFrame;

private:
	//bool judge();
	void erase();

	static VideoController *instance;

	cv::Mat mBackground;
	cv::Mat mFrame;
	cv::Mat mTmpFrame;

	//VideoPlayer mVideoPlayer[ANIMATION_NUM];
	cv::VideoCapture mVideoCapture;
	cv::Mat mMixFrame;
	cv::Mat tmpGrayMixFrame;
	cv::Mat tmpBackGround;
	cv::Mat tmpDepthFrame;
	cv::Mat fog;
	vector<cv::Mat> fogChannels;
	cv::Mat brush;							//单通道灰度图

	int x = 0;
	int y = 0;
	int pixelNum = VIDEO_SIZE.width * VIDEO_SIZE.height;
	bool doseErase = false;
	State mState = before;
};



#pragma once

#include"Util.h"



#define ANIMATION_NUM 1

const cv::Size VIDEO_SIZE = cv::Size(640, 480);

class VideoPlayer
{
public:
	VideoPlayer() {}
	VideoPlayer(cv::Point point, cv::String videoName) : mPoint(point)
	{
		mVideoCapture = cv::VideoCapture(videoName);
		if (!mVideoCapture.isOpened())
		{
			cout << "open video failed" << endl;
		}
	}

	void getFrame()
	{
		if (!isPlaying)
			return;

		if (!mVideoCapture.read(frame))
		{
			isPlaying = false;
			mVideoCapture.set(cv::CAP_PROP_POS_FRAMES, 0);
			return;
		}

		cv::resize(frame, frame, VIDEO_SIZE);
	}

	cv::VideoCapture mVideoCapture;
	bool isPlaying = false;
	cv::Point mPoint;
	cv::Mat frame;

private:

};


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

	cv::Mat mDepthFrame;

private:
	bool judge();

	static VideoController *instance;

	cv::Mat mBackground;

	VideoPlayer mVideoPlayer[ANIMATION_NUM];
	cv::Mat mMixFrame;
	cv::Mat tmpGrayMixFrame;
	cv::Mat tmpBackGround;
	cv::Mat tmpDepthFrame;
	cv::Mat fog;

	int x = 0;
	int y = 0;
	bool doseErase = false;
};



#include "VideoController.h"

using namespace cv;


#define MIN_HEIGHT 10
#define BRUSH_SIZE Size(136,128)
#define FINAL_SIZE Size(1920,1080)

VideoController::VideoController()
{
	//初始化笔刷
	Mat tmpBrush = imread("brush/brush.png", IMREAD_UNCHANGED);
	resize(tmpBrush, tmpBrush, BRUSH_SIZE);

	if (tmpBrush.channels() == 4)
		cvtColor(tmpBrush, tmpBrush, COLOR_BGRA2GRAY);
	else if (tmpBrush.channels() == 3)
		cvtColor(tmpBrush, tmpBrush, COLOR_BGR2GRAY);

	brush = tmpBrush;

	//初始化所有待触发的区域
	//mVideoPlayer[0] = VideoPlayer("res/Bridge.mp4");
	mVideoCapture = VideoCapture("res/Bridge2.mp4");
	if (!mVideoCapture.isOpened())
	{
		cout << "open video failed" << endl;
	}

	mVideoCapture.set(cv::CAP_PROP_POS_FRAMES, 2);

	//初始化所有图像
	//mMixFrame = Mat::zeros(VIDEO_SIZE, CV_8UC3);

	//mBackground = imread("res/background.jpg");

	mVideoCapture.read(mFrame);
	resize(mFrame, mFrame, VIDEO_SIZE);

	fog = imread("res/fog.png", IMREAD_UNCHANGED);
	resize(fog, fog, VIDEO_SIZE);

	split(fog, fogChannels);
}

VideoController::~VideoController()
{
	//for (size_t i = 0; i < ANIMATION_NUM; i++)
	//{
	//	mVideoPlayer->mVideoCapture.release();
	//}
	mVideoCapture.release();

	destroyAllWindows();
}

VideoController* VideoController::getVideoController()
{
	return instance;
}

void VideoController::play()
{
	switch (mState) {
	case before:
		erase();

		if (countNonZero(fogChannels[3]) < pixelNum * 0.5)
		{
			mState = playing;
			mciSendString(TEXT("open D:/222.mp3 alias mysong"), NULL, 0, NULL);
			mciSendString(TEXT("play mysong"), NULL, 0, NULL);
		}
		break;
	case playing:
		if (!mVideoCapture.read(mFrame))
		{
			mVideoCapture.set(cv::CAP_PROP_POS_FRAMES, 2);
			mVideoCapture.read(mFrame);
			mState = after;
			mciSendString(TEXT("close mysong"), NULL, 0, NULL);
		}
		cv::resize(mFrame, mFrame, VIDEO_SIZE);

		//腐蚀操作
		if(countNonZero(fogChannels[3])>pixelNum*0.1)
			erode(fogChannels[3], fogChannels[3], Mat());

		break;
	case after:
		//膨胀
		dilate(fogChannels[3], fogChannels[3], Mat());

		if (countNonZero(fogChannels[3]) >= pixelNum * 0.99)
			mState = before;
		break;
	}

	vector<Mat> tmpChannels;

	split(mFrame, tmpChannels);

	Mat invAlpha = 255 - fogChannels[3];

	for (int i = 0; i < 3; i++)
	{
		tmpChannels[i] = tmpChannels[i].mul(invAlpha, 1.0 / 255);
		tmpChannels[i] += fogChannels[i].mul(fogChannels[3], 1.0 / 255);
	}

	merge(tmpChannels, mTmpFrame);
	
	resize(mTmpFrame, mTmpFrame, FINAL_SIZE);

	//黑色遮罩
	//normalize(mDepthFrame, tmpDepthFrame, 0.2, 1.0, NORM_MINMAX,CV_16FC1);
	//Util::addWeighted(tmpBackGround, tmpDepthFrame, tmpBackGround);
	
	imshow("test", mTmpFrame);
}

void VideoController::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
	doseErase = true;
}

void VideoController::finishVideo()
{
	fogChannels[3] = fogChannels[0].clone();
}

//判断是否触发动画
//bool VideoController::judge()
//{
//	bool flag = false;
//
//	for (size_t i = 0; i < ANIMATION_NUM; i++)
//	{
//		//给定区域有人出现
//		//if (mDepthFrame.at<unsigned short>(mVideoPlayer[i].mPoint) > MIN_HEIGHT && !mVideoPlayer[i].isPlaying)
//		//	mVideoPlayer[i].isPlaying = true;
//
//		//雾气擦除到60%
//		if (countNonZero(fogChannels[3]) < pixelNum * 0.4)
//			mVideoPlayer[i].isPlaying = true;
//
//		mVideoPlayer[i].getFrame();
//
//		if (mVideoPlayer[i].isPlaying)
//		{
//			mMixFrame += mVideoPlayer[i].frame;
//			flag = true;
//		}
//	}
//
//	return flag;
//}

void VideoController::erase()
{
	//白色雾气遮罩
	if (doseErase) {
		//Util::erase(fog, fog, x, y);

		int lefttopX = 0;
		int lefttopY = 0;

		int brushX = 0;
		int brushY = 0;
		int brushWidth = brush.cols;
		int brushHeight = brush.rows;

		//左边界
		if (x > brush.cols / 2)
			lefttopX = x - brush.cols / 2;
		else if (x > - brush.cols / 2) {
			brushX = brush.cols / 2 - x;
			brushWidth = brush.cols - brushX;
		}
		else{
			return;
		}

		//右边界
		if (x >= fogChannels[3].cols - brush.cols / 2)
			brushWidth = fogChannels[3].cols - lefttopX;

		//上边界
		if (y > brush.rows / 2)
			lefttopY = y - brush.rows / 2;
		else if(y> -brush.rows / 2){
			brushY = brush.rows / 2 - y;
			brushHeight = brush.rows - brushY;
		}
		else {
			return;
		}

		//下边界
		if (y >= fogChannels[3].rows - brush.rows / 2)
			brushHeight = fogChannels[3].rows - lefttopY;

		Rect srcROI(lefttopX, lefttopY, brushWidth, brushHeight);
		Rect brushROI(brushX, brushY, brushWidth, brushHeight);

		//alpha通道
		//srcChannels[3](srcROI) = srcChannels[3](srcROI).mul(brush(brushROI), 1.0 / 255);

		fogChannels[3](srcROI) -= brush(brushROI);

		doseErase = false;
	}
}

VideoController* VideoController::instance = new VideoController();

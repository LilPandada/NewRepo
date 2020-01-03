#include "VideoController.h"

using namespace cv;


#define MIN_HEIGHT 10

VideoController::VideoController()
{
	//��ʼ�����д�����������
	mVideoPlayer[0] = VideoPlayer(Point2i(230, 150), "res/1-1.avi");

	//��ʼ��Mat
	mMixFrame = Mat::zeros(VIDEO_SIZE, CV_8UC3);

	mBackground = imread("res/background.jpg");
	if (mBackground.empty())
		cout << "imread failed" << endl;
	else
		resize(mBackground, mBackground, VIDEO_SIZE);

	fog = imread("res/fog.png", IMREAD_UNCHANGED);
	resize(fog, fog, mBackground.size());
}

VideoController::~VideoController()
{
	for (size_t i = 0; i < ANIMATION_NUM; i++)
	{
		mVideoPlayer->mVideoCapture.release();
	}

	destroyAllWindows();
}

VideoController* VideoController::getVideoController()
{
	return instance;
}

void VideoController::play()
{
	tmpBackGround = mBackground.clone();

	if (judge())
	{
		mMixFrame = ~mMixFrame;

		//�õ�����
		cvtColor(mMixFrame, tmpGrayMixFrame, COLOR_BGR2GRAY);

		mMixFrame.copyTo(tmpBackGround, tmpGrayMixFrame);
	}

	//��ɫ��������
	if (doseErase) {
		Util::erase(fog, fog, x, y);
		doseErase = false;
	}

	Util::mix(tmpBackGround, fog, tmpBackGround);
	
	//��ɫ����
	//normalize(mDepthFrame, tmpDepthFrame, 0.2, 1.0, NORM_MINMAX,CV_16FC1);
	//Util::addWeighted(tmpBackGround, tmpDepthFrame, tmpBackGround);
	
	imshow("test", tmpBackGround);
}

void VideoController::setXY(int x, int y)
{
	this->x = x;
	this->y = y;
	doseErase = true;
}

//�ж��Ƿ񴥷�����
bool VideoController::judge()
{
	bool flag = false;

	//for (size_t i = 0; i < ANIMATION_NUM; i++)
	//{
	//	if (mDepthFrame.at<UINT16>(mVideoPlayer[i].mPoint) > MIN_HEIGHT && !mVideoPlayer[i].isPlaying)
	//		mVideoPlayer[i].isPlaying = true;

	//	mVideoPlayer[i].getFrame();

	//	if (mVideoPlayer[i].isPlaying)
	//	{
	//		mMixFrame += mVideoPlayer[i].frame;
	//		flag = true;
	//	}
	//}

	return flag;
}

VideoController* VideoController::instance = new VideoController();

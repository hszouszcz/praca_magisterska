#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <chrono>
#include "Mouse.h"

using namespace std;
using namespace cv;

extern MouseState mouseState;

	
class EyeTracker
{
public:
	String face_cascade_name = "haarcascade_frontalface_alt.xml";
	String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
	String nose_cascade_name = "haarcascade_mcs_nose.xml";
	CascadeClassifier face_cascade2;
	CascadeClassifier eyes_cascade2;
	CascadeClassifier nose_cascade2;
	string window_name = "Capture - Face detection";
	enum EyeRegion { LEFT, RIGHT };

	EyeTracker();
	~EyeTracker();

	bool are_eyes_detected(std::vector<cv::Rect> eyes, cv::Point reference_point);
	double blink_duration(std::chrono::high_resolution_clock::time_point &last_seen);
	EyeRegion recognize_eye_region(cv::Point reference_point, cv::Point eye_point);
	void track_eyes_and_blinks(Mat frame);
	void EyeTracker::detectAndDisplay(IplImage* frame, CvPoint headCorner, int headWidth, int headHeight);
};


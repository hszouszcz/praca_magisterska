///**
//* eye-tracking.cpp:
//* Eye detection and tracking with OpenCV
//*
//* This program tries to detect and tracking the user's eye with webcam.
//* At startup, the program performs face detection followed by eye detection
//* using OpenCV's built-in Haar cascade classifier. If the user's eye detected
//* successfully, an eye template is extracted. This template will be used in
//* the subsequent template matching for tracking the eye.
//*/
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/objdetect/objdetect.hpp>
//#include <windows.h>
//
//cv::CascadeClassifier face_cascade;
//cv::CascadeClassifier eye_cascade;
//
///**
//* Function to detect human face and the eyes from an image.
//*
//* @param  im    The source image
//* @param  tpl   Will be filled with the eye template, if detection success.
//* @param  rect  Will be filled with the bounding box of the eye
//* @return zero=failed, nonzero=success
//*/
//int detectEye(cv::Mat& im, cv::Mat& tpl, cv::Rect& rect)
//{
//	std::vector<cv::Rect> faces, eyes;
//	face_cascade.detectMultiScale(im, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
//
//	for (int i = 0; i < faces.size(); i++)
//	{
//		cv::Mat face = im(faces[i]);
//		eye_cascade.detectMultiScale(face, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));
//
//		if (eyes.size())
//		{
//			rect = eyes[0] + cv::Point(faces[i].x, faces[i].y);
//			tpl = im(rect);
//		}
//	}
//
//	return eyes.size();
//}
//
///**
//* Perform template matching to search the user's eye in the given image.
//*
//* @param   im    The source image
//* @param   tpl   The eye template
//* @param   rect  The eye bounding box, will be updated with the new location of the eye
//*/
//void trackEye(cv::Mat& im, cv::Mat& tpl, cv::Rect& rect)
//{
//	cv::Size size(rect.width * 2, rect.height * 2);
//	cv::Rect window(rect + size - cv::Point(size.width / 2, size.height / 2));
//
//	window &= cv::Rect(0, 0, im.cols, im.rows);
//
//	cv::Mat dst(window.width - tpl.rows + 1, window.height - tpl.cols + 1, CV_32FC1);
//	cv::matchTemplate(im(window), tpl, dst, CV_TM_SQDIFF_NORMED);
//
//	double minval, maxval;
//	cv::Point minloc, maxloc;
//	cv::minMaxLoc(dst, &minval, &maxval, &minloc, &maxloc);
//
//	if (minval <= 0.2)
//	{
//		rect.x = window.x + minloc.x;
//		rect.y = window.y + minloc.y;
//	}
//	else
//		rect.x = rect.y = rect.width = rect.height = 0;
//}
//
//int main(int argc, char** argv)
//{
//	// Load the cascade classifiers
//	// Make sure you point the XML files to the right path, or 
//	// just copy the files from [OPENCV_DIR]/data/haarcascades directory
//	face_cascade.load("haarcascade_frontalface_alt2.xml");
//	eye_cascade.load("haarcascade_mcs_lefteye.xml");
//
//	// Open webcam
//	cv::VideoCapture cap(0);
//	Sleep(2000);
//	// Check if everything is ok
//	if (face_cascade.empty() || eye_cascade.empty() || !cap.isOpened())
//		return 1;
//
//	// Set video to 320x240
//	cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
//	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
//
//	cv::Mat frame, eye_tpl;
//	cv::Rect eye_bb;
//
//	while (cv::waitKey(15) != 'q')
//	{
//		cap >> frame;
//		if (frame.empty())
//			break;
//
//		// Flip the frame horizontally, Windows users might need this
//		cv::flip(frame, frame, 1);
//
//		// Convert to grayscale and 
//		// adjust the image contrast using histogram equalization
//		cv::Mat gray;
//		cv::cvtColor(frame, gray, CV_BGR2GRAY);
//
//		if (eye_bb.width == 0 && eye_bb.height == 0)
//		{
//			// Detection stage
//			// Try to detect the face and the eye of the user
//			detectEye(gray, eye_tpl, eye_bb);
//		}
//		else
//		{
//			// Tracking stage with template matching
//			trackEye(gray, eye_tpl, eye_bb);
//
//			// Draw bounding rectangle for the eye
//			cv::rectangle(frame, eye_bb, CV_RGB(0, 255, 0));
//		}
//
//		// Display video
//		cv::imshow("video", frame);
//	}
//
//	return 0;
//}



//
//
//
//
//
//#include <opencv2/objdetect/objdetect.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//#include <iostream>
//#include <queue>
//#include <stdio.h>
//#include <math.h>
//#include <windows.h>
//#include "Constans.h"
//#include "findEyeCenter.h"
//#include "findEyeCorner.h"
//
//
///** Constants **/
//
//
///** Function Headers */
//void detectAndDisplay(cv::Mat frame);
//
///** Global variables */
////-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations
//cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
//cv::CascadeClassifier face_cascade;
//std::string main_window_name = "Capture - Face detection";
//std::string face_window_name = "Capture - Face";
//cv::RNG rng(12345);
//cv::Mat debugImage;
//cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);
//
///**
//* @function main
//*/
//int main(int argc, const char** argv) {
//	CvCapture* capture;
//	cv::Mat frame;
//
//	// Load the cascades
//	if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading face cascade, please change face_cascade_name in source code.\n"); return -1; };
//
//	cv::namedWindow(main_window_name, CV_WINDOW_NORMAL);
//	cv::moveWindow(main_window_name, 400, 100);
//	cv::namedWindow(face_window_name, CV_WINDOW_NORMAL);
//	cv::moveWindow(face_window_name, 10, 100);
//	cv::namedWindow("Right Eye", CV_WINDOW_NORMAL);
//	cv::moveWindow("Right Eye", 10, 600);
//	cv::namedWindow("Left Eye", CV_WINDOW_NORMAL);
//	cv::moveWindow("Left Eye", 10, 800);
//	/*cv::namedWindow("aa", CV_WINDOW_NORMAL);
//	cv::moveWindow("aa", 10, 800);
//	cv::namedWindow("aaa", CV_WINDOW_NORMAL);
//	cv::moveWindow("aaa", 10, 800);*/
//
//	createCornerKernels();
//	ellipse(skinCrCbHist, cv::Point(113, 155.6), cv::Size(23.4, 15.2),
//		43.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);
//
//	// Read the video stream
//	cv::VideoCapture cap;
//	cap.open(0);
//	
//	//capture = cvCaptureFromCAM(-1);
//	Sleep(1000);
//	if (cap.isOpened()) {
//		
//		while (true) {
//			cap >> frame;
//			// mirror it
//			cv::flip(frame, frame, 1);
//			frame.copyTo(debugImage);
//
//			// Apply the classifier to the frame
//			if (!frame.empty()) {
//				detectAndDisplay(frame);
//			}
//			else {
//				printf(" --(!) No captured frame -- Break!");
//				break;
//			}
//
//			imshow(main_window_name, debugImage);
//
//			int c = cv::waitKey(10);
//			if ((char)c == 'c') { break; }
//			if ((char)c == 'f') {
//				imwrite("frame.png", frame);
//			}
//
//		}
//	}
//
//	releaseCornerKernels();
//
//	return 0;
//}
//
//void findEyes(cv::Mat frame_gray, cv::Rect face) {
//	cv::Mat faceROI = frame_gray(face);
//	cv::Mat debugFace = faceROI;
//
//	if (kSmoothFaceImage) {
//		double sigma = kSmoothFaceFactor * face.width;
//		GaussianBlur(faceROI, faceROI, cv::Size(0, 0), sigma);
//	}
//	//-- Find eye regions and draw them
//	int eye_region_width = face.width * (kEyePercentWidth / 100.0);
//	int eye_region_height = face.width * (kEyePercentHeight / 100.0);
//	int eye_region_top = face.height * (kEyePercentTop / 100.0);
//	cv::Rect leftEyeRegion(face.width*(kEyePercentSide / 100.0),
//		eye_region_top, eye_region_width, eye_region_height);
//	cv::Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentSide / 100.0),
//		eye_region_top, eye_region_width, eye_region_height);
//
//	//-- Find Eye Centers
//	cv::Point leftPupil = findEyeCenter(faceROI, leftEyeRegion, "Left Eye");
//	cv::Point rightPupil = findEyeCenter(faceROI, rightEyeRegion, "Right Eye");
//	// get corner regions
//	cv::Rect leftRightCornerRegion(leftEyeRegion);
//	leftRightCornerRegion.width -= leftPupil.x;
//	leftRightCornerRegion.x += leftPupil.x;
//	leftRightCornerRegion.height /= 2;
//	leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
//	cv::Rect leftLeftCornerRegion(leftEyeRegion);
//	leftLeftCornerRegion.width = leftPupil.x;
//	leftLeftCornerRegion.height /= 2;
//	leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
//	cv::Rect rightLeftCornerRegion(rightEyeRegion);
//	rightLeftCornerRegion.width = rightPupil.x;
//	rightLeftCornerRegion.height /= 2;
//	rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
//	cv::Rect rightRightCornerRegion(rightEyeRegion);
//	rightRightCornerRegion.width -= rightPupil.x;
//	rightRightCornerRegion.x += rightPupil.x;
//	rightRightCornerRegion.height /= 2;
//	rightRightCornerRegion.y += rightRightCornerRegion.height / 2;
//	rectangle(debugFace, leftRightCornerRegion, 200);
//	rectangle(debugFace, leftLeftCornerRegion, 200);
//	rectangle(debugFace, rightLeftCornerRegion, 200);
//	rectangle(debugFace, rightRightCornerRegion, 200);
//	// change eye centers to face coordinates
//	rightPupil.x += rightEyeRegion.x;
//	rightPupil.y += rightEyeRegion.y;
//	std::cout << "rightPupil.x" << " " << rightPupil.x << std::endl;
//	std::cout << "rightPupil.y" << "        " << rightPupil.y << std::endl;
//	leftPupil.x += leftEyeRegion.x;
//	leftPupil.y += leftEyeRegion.y;
//	// draw eye centers
//	circle(debugFace, rightPupil, 3, 1234);
//	circle(debugFace, leftPupil, 3, 1234);
//
//	//-- Find Eye Corners
//	if (kEnableEyeCorner) {
//		cv::Point2f leftRightCorner = findEyeCorner(faceROI(leftRightCornerRegion), true, false);
//		leftRightCorner.x += leftRightCornerRegion.x;
//		leftRightCorner.y += leftRightCornerRegion.y;
//		cv::Point2f leftLeftCorner = findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
//		leftLeftCorner.x += leftLeftCornerRegion.x;
//		leftLeftCorner.y += leftLeftCornerRegion.y;
//		cv::Point2f rightLeftCorner = findEyeCorner(faceROI(rightLeftCornerRegion), false, true);
//		rightLeftCorner.x += rightLeftCornerRegion.x;
//		rightLeftCorner.y += rightLeftCornerRegion.y;
//		cv::Point2f rightRightCorner = findEyeCorner(faceROI(rightRightCornerRegion), false, false);
//		rightRightCorner.x += rightRightCornerRegion.x;
//		rightRightCorner.y += rightRightCornerRegion.y;
//		circle(faceROI, leftRightCorner, 3, 200);
//		circle(faceROI, leftLeftCorner, 3, 200);
//		circle(faceROI, rightLeftCorner, 3, 200);
//		circle(faceROI, rightRightCorner, 3, 200);
//	}
//
//	imshow(face_window_name, faceROI);
//	//  cv::Rect roi( cv::Point( 0, 0 ), faceROI.size());
//	//  cv::Mat destinationROI = debugImage( roi );
//	//  faceROI.copyTo( destinationROI );
//}
//
//
//cv::Mat findSkin(cv::Mat &frame) {
//	cv::Mat input;
//	cv::Mat output = cv::Mat(frame.rows, frame.cols, CV_8U);
//
//	cvtColor(frame, input, CV_BGR2YCrCb);
//
//	for (int y = 0; y < input.rows; ++y) {
//		const cv::Vec3b *Mr = input.ptr<cv::Vec3b>(y);
//		//    uchar *Or = output.ptr<uchar>(y);
//		cv::Vec3b *Or = frame.ptr<cv::Vec3b>(y);
//		for (int x = 0; x < input.cols; ++x) {
//			cv::Vec3b ycrcb = Mr[x];
//			//      Or[x] = (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0) ? 255 : 0;
//			if (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) == 0) {
//				Or[x] = cv::Vec3b(0, 0, 0);
//			}
//		}
//	}
//	return output;
//}
//
///**
//* @function detectAndDisplay
//*/
//void detectAndDisplay(cv::Mat frame) {
//	std::vector<cv::Rect> faces;
//	//cv::Mat frame_gray;
//
//	std::vector<cv::Mat> rgbChannels(3);
//	cv::split(frame, rgbChannels);
//	cv::Mat frame_gray = rgbChannels[2];
//
//	//cvtColor( frame, frame_gray, CV_BGR2GRAY );
//	//equalizeHist( frame_gray, frame_gray );
//	//cv::pow(frame_gray, CV_64F, frame_gray);
//	//-- Detect faces
//	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(150, 150));
//	//  findSkin(debugImage);
//
//	for (int i = 0; i < faces.size(); i++)
//	{
//		rectangle(debugImage, faces[i], 1234);
//	}
//	//-- Show what you got
//	if (faces.size() > 0) {
//		findEyes(frame_gray, faces[0]);
//	}
//}
//
//
//
//
//
//
//
//


// Blink Detection.cpp : Defines the entry point for the console application.
//


#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <chrono>


using namespace std::chrono;
using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat frame);
bool isValidEye(int referenceX, int eyeX, int prevEyeX = 0);
/** Global variables */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
String nose_cascade_name = "haarcascade_mcs_nose.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
CascadeClassifier nose_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);


enum EyeRegion { LEFT, RIGHT };
bool are_eyes_detected(vector<Rect> eyes, Point reference_point);
EyeRegion recognize_eye_region(Point reference_point, Point eye_point);
double blink_duration(high_resolution_clock::time_point &last_seen);
/** @function main */
int main(int argc, const char** argv)
{
	CvCapture* capture;
	Mat frame;

	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading\n"); return -1; };
	if (!eyes_cascade.load(eyes_cascade_name)){ printf("--(!)Error loading\n"); return -1; };
	if (!nose_cascade.load(nose_cascade_name)) { printf("--(!)Error loading\n"); return -1; };

	//-- 2. Read the video stream
	VideoCapture cap;
		cap.open(0);
	
	Sleep(1000);
	if (cap.isOpened())
	{
		while (true)
		{
			cap >> frame;

			//-- 3. Apply the classifier to the frame
			if (!frame.empty())
			{
				detectAndDisplay(frame);
			}
			else
			{
				printf(" --(!) No captured frame -- Break!"); break;
			}

			int c = waitKey(10);
			if ((char)c == 'c') { break; }
		}
	}
	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	high_resolution_clock::time_point last_seen_left_eye = high_resolution_clock::now() ,
		last_seen_right_eye = high_resolution_clock::now();;
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		line(frame, Point(faces[i].width / 2 + faces[i].x, faces[i].y - 300), Point(faces[i].width / 2 + faces[i].x, faces[i].y + 300), Scalar(255, 100, 255));
		ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;
		Point faceCenter = center;
		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	

		for (size_t j = 0; j < eyes.size(); j++)
		{
		
			/*Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);*/
			if (eyes.size() <= 2 && eyes.size() > 0) {
				if (recognize_eye_region(faceCenter, Point(eyes[j].x + faces[i].x, eyes[j].y)) == LEFT)
				{
					blink_duration(last_seen_left_eye);
					Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
					int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
					circle(frame, center, radius, Scalar(0, 255, 0), 4, 8, 0);
				}
				else if (recognize_eye_region(faceCenter, Point(eyes[j].x + faces[i].x, eyes[j].y)) == RIGHT)
				{
					blink_duration(last_seen_right_eye);
					Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
					int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
					circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
				}
			}
		}
	}
	//-- Show what you got
	imshow(window_name, frame);
}

EyeRegion recognize_eye_region(Point reference_point, Point eye_point)
{
	if (eye_point.x < reference_point.x)
		return LEFT;

	return RIGHT;

}


bool are_eyes_detected(vector<Rect> eyes, Point reference_point)
{
	Point firstEye = Point(eyes[0].x, eyes[0].y);
	Point secondEye = Point(eyes[1].x, eyes[1].y);

	if (eyes.size() == 2 && (recognize_eye_region(reference_point, firstEye) != recognize_eye_region(reference_point, secondEye)))
		return true;

	return false;
}

double blink_duration(high_resolution_clock::time_point &last_seen)
{
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	double duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - last_seen).count();
	last_seen = t1;
	std::cout << duration << endl;
	return duration;
}

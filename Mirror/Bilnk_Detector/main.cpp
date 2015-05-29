

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
					double duration = blink_duration(last_seen_left_eye);
					if (duration >900 && duration < 1300)
					{
						mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}
					
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

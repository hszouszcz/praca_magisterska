#include "EyeTracker.h"
#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
EyeTracker::EyeTracker()
{
}


EyeTracker::~EyeTracker()
{
}


bool EyeTracker::are_eyes_detected(std::vector<cv::Rect> eyes, cv::Point reference_point)
{
	cv::Point firstEye = cv::Point(eyes[0].x, eyes[0].y);
	cv::Point secondEye = cv::Point(eyes[1].x, eyes[1].y);

	if (eyes.size() == 2 && (recognize_eye_region(reference_point, firstEye) != recognize_eye_region(reference_point, secondEye)))
		return true;

	return false;
}


double EyeTracker::blink_duration(std::chrono::high_resolution_clock::time_point &last_seen)
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	double duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - last_seen).count();
	last_seen = t1;
	std::cout << duration << std::endl;
	return duration;
}

EyeTracker::EyeRegion EyeTracker:: recognize_eye_region(cv::Point reference_point, cv::Point eye_point)
{
	if (eye_point.x < reference_point.x)
		return LEFT;

	return RIGHT;

}

void EyeTracker::track_eyes_and_blinks(Mat frame)
{
	
	while (true)
	{

		//-- 3. Apply the classifier to the frame
		if (!frame.empty())
		{
			//detectAndDisplay(frame);
		}
		//else
		//{
		//	printf(" --(!) No captured frame -- Break!"); break;
		
	}

}


void EyeTracker:: detectAndDisplay(IplImage* frame, CvPoint headCorner, int headWidth, int headHeight)
{


	Mat(frame, true);
	Mat frame2 = frame;
	Mat frame3 = frame2(Rect(1, 1, 1, 1));

	chrono::high_resolution_clock::time_point last_seen_left_eye = chrono::high_resolution_clock::now(),
		last_seen_right_eye = chrono::high_resolution_clock::now();;
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame3, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	 Rect roi(headCorner.x, headCorner.y, headWidth, headHeight);
	 frame_gray = frame_gray(roi);

	//-- Detect faces
	//face_cascade2.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	//for (size_t i = 0; i < faces.size(); i++)
	//{
		//Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		//line(frame3, Point(faces[i].width / 2 + faces[i].x, faces[i].y - 300), Point(faces[i].width / 2 + faces[i].x, faces[i].y + 300), Scalar(255, 100, 255));
		//ellipse(frame3, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		//Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;
		Point faceCenter;
		int faceCenterX = headCorner.x + headWidth / 2;
		int faceCenterY = headCorner.y + headHeight / 2;
		faceCenter.x = faceCenterX;
		faceCenter.y = faceCenterY;
		//Point faceCenter = center;
		//-- In each face, detect eyes

		eyes_cascade2.detectMultiScale(frame_gray, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));


		for (size_t j = 0; j < eyes.size(); j++)
		{

			/*Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);*/
			if (eyes.size() <= 2 && eyes.size() > 0) {
				if (recognize_eye_region(faceCenter, Point(eyes[j].x + headCorner.x, eyes[j].y)) == EyeTracker::LEFT)
				{
					double duration = blink_duration(last_seen_left_eye);
					if (duration >900 && duration < 1300 && mouseState == CLICKABLE)
					{
						mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
					}

					Point center(faceCenter.x + eyes[j].x + eyes[j].width*0.5, faceCenter.y + eyes[j].y + eyes[j].height*0.5);
					int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
					circle(frame3, center, radius, Scalar(0, 255, 0), 4, 8, 0);
				}
				else if (recognize_eye_region(faceCenter, Point(eyes[j].x + faceCenter.x, eyes[j].y)) == EyeTracker::RIGHT)
				{
					blink_duration(last_seen_right_eye);
					Point center(faceCenter.x + eyes[j].x + eyes[j].width*0.5, faceCenter.y + eyes[j].y + eyes[j].height*0.5);
					int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
					circle(frame3, center, radius, Scalar(255, 0, 0), 4, 8, 0);
				}
			}
		}
	//}
	//-- Show what you got
	imshow(window_name, frame3);
}
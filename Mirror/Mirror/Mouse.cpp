#include "Mouse.h"
#define NOMINMAX
#include "windows.h"
#include <iostream>

MouseState mouseState;

void ::Mouse::increment_x()
{
	x+=5;
}

void Mouse::increment_y()
{
	y-=5;
}

void Mouse::decrement_x()
{
	x+=5;
}

void Mouse::decrement_y()
{
	y-=5;
}

void Mouse::move_mouse(double euler_x, double euler_y)
{
	
	mouseState = CLICKABLE;
	if (euler_x < 3.005 && euler_x > 2.8)
	{
		SetCursorPos(x, y);
		decrement_y();
		mouseState = IN_MOVE;
		
	}
	if (euler_x > 3.3)
	{
		SetCursorPos(x, y);
		increment_y();
		mouseState = IN_MOVE;
		
	}
	if (euler_y < -0.2)
	{
		SetCursorPos(x, y);
		decrement_x();
		mouseState = IN_MOVE;
		
	};
	if (euler_y > 0)
	{
		SetCursorPos(x, y);
		increment_x();
		mouseState = IN_MOVE;
		
	}
	
	

}

//bool Mouse::are_eyes_detected(std::vector<cv::Rect> eyes, cv::Point reference_point)
//{
//	cv::Point firstEye = cv::Point(eyes[0].x, eyes[0].y);
//	cv::Point secondEye = cv::Point(eyes[1].x, eyes[1].y);
//
//	if (eyes.size() == 2 && (recognize_eye_region(reference_point, firstEye) != recognize_eye_region(reference_point, secondEye)))
//		return true;
//
//	return false;
//}
//
//
//double Mouse::blink_duration(std::chrono::high_resolution_clock::time_point &last_seen)
//{
//	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
//	double duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - last_seen).count();
//	last_seen = t1;
//	std::cout << duration << std::endl;
//	return duration;
//}

Mouse::Mouse()
{
	x = GetSystemMetrics(SM_CXSCREEN)/2;
	y = GetSystemMetrics(SM_CYSCREEN)/2;
}




Mouse::~Mouse()
{
}

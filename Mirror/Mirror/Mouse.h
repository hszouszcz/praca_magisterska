#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <vector>
#include <chrono>
#include <cv.hpp>

enum MouseState {CLICKABLE, IN_MOVE};

class Mouse
{
public:
	int x1() const
	{
		return x;
	}

	void set_x(int x1)
	{
		x = x1;
	}

	int y1() const
	{
		return y;
	}

	void set_y(int y1)
	{
		y = y1;
	}

	void increment_x();
	void increment_y();
	void decrement_x();
	void decrement_y();
	void move_mouse(double euler_x, double euler_y);

	//bool are_eyes_detected(std::vector<cv::Rect> eyes, cv::Point reference_point);
	//double blink_duration(std::chrono::high_resolution_clock::time_point &last_seen);
	

private:
	int x;
	int y;
	
public:
	Mouse();
	~Mouse();
};


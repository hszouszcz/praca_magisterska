#include "Mouse.h"
#include "windows.h"
#include <iostream>

void ::Mouse::increment_x()
{
	x+=5;
}

void Mouse::increment_y()
{
	y+=5;
}

void Mouse::decrement_x()
{
	x-=5;
}

void Mouse::decrement_y()
{
	y-=5;
}

void Mouse::move_mouse(double euler_x, double euler_y)
{
	static int c = 0;
	if (euler_x < 3.005 && euler_x > 2.8)
	{
		SetCursorPos(x, y);
		decrement_y();
	}
	if (euler_x > 3.3)
	{
		SetCursorPos(x, y);
		increment_y();
	}
	if (euler_y < -0.2)
	{
		SetCursorPos(x, y);
		decrement_x();
	};
	if (euler_y > 0)
	{
		SetCursorPos(x, y);
		increment_x();
	}
	c++;
	//(lpPoint);
	

}

Mouse::Mouse()
{
	x = GetSystemMetrics(SM_CXSCREEN)/2;
	y = GetSystemMetrics(SM_CYSCREEN)/2;
}




Mouse::~Mouse()
{
}

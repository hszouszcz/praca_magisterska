#pragma once
#include <windows.h>

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
	

private:
	int x;
	int y;
	LPPOINT lpPoint;
public:
	Mouse();
	~Mouse();
};


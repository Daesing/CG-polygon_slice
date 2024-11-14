#pragma once
#include "header.h"


class Basket {
public:
	float obj[4][3]{
		{-0.2f,-0.1f,0.0f},{0.2f,-0.1f,0.0f},{-0.2f,0.1f,0.0f},{0.2f,0.1f,0.0f}
	};
	float x_translation{ 0 };
	float y_translation{ -0.7 };
	float v{ 0.1 };
	float color[4][3]{
	{1,0,0},{1,0,0},{1,0,0},{1,0,0}
	};
	int dir{ 1 };


	void move();
};

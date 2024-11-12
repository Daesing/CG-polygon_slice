#pragma once
#include "line.h"

class Shape {
public:
	float obj[4][3]{
		{-0.2f,-0.2f,0.0f},{0.2f,-0.2f,0.0f},{-0.2f,0.2f,0.0f},{0.2f,0.2f,0.0f}
	};

	float obj_cord[4][3]{};

	float color[4][3]{
	{0,0,0},{0,0,0},{0,1,0},{0,1,0}
	};

	bool is_active{ true };
	float x_translation{ 4 };
	float y_translation{ -2 };

	float vx{ 0.1 }, vy{ 0.05 };
	int shape_type{};
	int dir{};

	

	void move();
	void reset_shape(int,int);
	void set_pos();
	void is_split(Line line);
	void update_pos();

};



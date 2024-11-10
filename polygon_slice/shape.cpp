#pragma once
#include "shape.h"



void Shape::reset_shape(int shape_type_,int dir_)
{
	shape_type = shape_type_;
	dir = dir_;
	set_pos();
}

void Shape::set_pos()
{
	if (dir == 1) {
		//LT
		x_translation = -2;
		y_translation = 1;
	}
	else if (dir == 2) {
		//RT
		x_translation = 2;
		y_translation = 1;
	}
	else if (dir == 3) {
		//LB
		x_translation = -2;
		y_translation = -1;
	}
	else {
		//RB
		x_translation = 2;
		y_translation = -1;
	}
}

void Shape::move() {

	if (dir == 1) {
		//To RB
		x_translation += vx;
		y_translation += -vy;
	}
	else if (dir == 2) {
		//To LB
		x_translation += -vx;
		y_translation += -vy;
	}
	else if (dir == 3) {
		//To RT
		x_translation += vx;
		y_translation += vy;
	}
	else {
		//To LT
		x_translation += -vx;
		y_translation += vy;
	}
}

void Shape::is_split(Line line) {
}
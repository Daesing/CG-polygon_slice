#include "func.h"


float convert_x(int x) {
	return ((float)x / (WIDTH / 2)) - 1.f;
}
float convert_y(int y) {
	return 1.f - ((float)y / (HEIGHT / 2));
}

void check_crushed(Line line, Shape& shape) {
	
	if (line_line(line.obj[0][0], line.obj[0][1], line.obj[1][0], line.obj[1][1], shape.obj[0][0], shape.obj[0][1], shape.obj[1][0], shape.obj[1][1]))
		shape.is_active = false;
	if(line_line(line.obj[0][0], line.obj[0][1], line.obj[1][0], line.obj[1][1], shape.obj[1][0], shape.obj[1][1], shape.obj[3][0], shape.obj[3][1]))
		shape.is_active = false;
	if(line_line(line.obj[0][0], line.obj[0][1], line.obj[1][0], line.obj[1][1], shape.obj[0][0], shape.obj[0][1], shape.obj[2][0], shape.obj[2][1]))
		shape.is_active = false;
	if(line_line(line.obj[0][0], line.obj[0][1], line.obj[1][0], line.obj[1][1], shape.obj[2][0], shape.obj[2][1], shape.obj[3][0], shape.obj[3][1]))
		shape.is_active = false;

}

// LINE/LINE
bool line_line(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {


	// calculate the distance to intersection point
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

		// optionally, draw a circle where the lines meet
		float intersectionX = x1 + (uA * (x2 - x1));
		float intersectionY = y1 + (uA * (y2 - y1));

		return true;
	}
	return false;
}



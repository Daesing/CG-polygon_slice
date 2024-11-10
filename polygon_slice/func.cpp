#include "func.h"


float convert_x(int x) {
	return ((float)x / (WIDTH / 2)) - 1.f;
}
float convert_y(int y) {
	return 1.f - ((float)y / (HEIGHT / 2));
}



// LINE/LINE
void line_line(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

	// calculate the distance to intersection point
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {

		// optionally, draw a circle where the lines meet
		float intersectionX = x1 + (uA * (x2 - x1));
		float intersectionY = y1 + (uA * (y2 - y1));
		

		return;
	}
	return;
}



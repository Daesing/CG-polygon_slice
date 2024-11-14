#pragma once
#include "header.h"


float convert_x(int x);
float convert_y(int y);
bool line_line(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

void check_crushed(Line , Shape&);
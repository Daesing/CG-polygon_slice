#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <GL/glew.h> // 필요한 헤더파일 include
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>
#include <random>
#include "split_shape.h"
#include "shape.h"
#include "func.h"
#include "line.h"

CONST INT WIDTH = 800;
CONST INT HEIGHT = 600;
CONST INT ARRAYSIZE = 5;

extern std::random_device rd;
extern std::default_random_engine dre;
extern std::uniform_int_distribution<int> uid_shape;
extern std::uniform_int_distribution<int> uid_dir;
extern std::uniform_real_distribution<float> urd_color;

extern Shape shape[ARRAYSIZE];
extern Line line;


void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
void InitBuffer();
char* filetobuf(const char* file);
void Keyboard(unsigned char key, int x, int y);
void TimerFunction(int value);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

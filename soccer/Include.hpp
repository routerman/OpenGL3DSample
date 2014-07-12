#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#ifdef _WIN32
	#include <windows.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#elif defined __unix__
	#include <GL/glut.h>
#elif defined __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/glut.h>
#endif

#define ESC 27

class Obj;
class Ball;
class Team;
class Player;

extern const int init_window_width;
extern const int init_window_height;
extern const int field_x;
extern const int field_y;
extern const int goal_y;
extern const int goal_z;
extern const double gravity;
extern const int max_player; //

extern class Ball *ball;
extern class Team *team[2];
extern class Player *player[22];




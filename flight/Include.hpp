#pragma once
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <list>
#include <functional>
#include <string>
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




using namespace std;

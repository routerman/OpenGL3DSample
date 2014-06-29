#include <stdlib.h>
//#include <time.h>
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
const int max_ball = 100; //
double gravity = 0;// -0.098;
double angle = 0.0; 
double theta = 0.0;
double zoom = 1.0;
int isDragging = 0;
int xDragStart = 0;
int yDragStart = 0;

class Ball{
private:
	double r, g, b;
	double radius;
	double x, y, z;
	double vx, vy, vz;
	double ax, ay, az;
	GLfloat gold_diff[4];

public:
	Ball(double radius, double x, double y, double z){
		this->radius = radius;
		this->x = x;
		this->y = y;
		this->z = z;
		vx = vy = vz = 0;
		ax = ay = az = 0;
		az = - g;
	}
	void setColor(double r, double g, double b){
		this->r = r;
		this->g = g;
		this->b = b;
		gold_diff[0] = r;// 0.75164;
		gold_diff[1] = g;// 0.60648;
		gold_diff[2] = b;// 0.22648;
		gold_diff[3] = 1.0;
	}

	void keycontrol(int key, bool press){
		switch (key) {
		case GLUT_KEY_RIGHT:
			if (press) ax = 0.5;
			else ax = 0.0;
			break;
		case GLUT_KEY_LEFT:
			if (press) ax = -0.5;
			else ax = 0.0;
			break;
		case GLUT_KEY_UP:
			if (press) ay = 0.5;
			else ay = 0;
			break;
		case GLUT_KEY_DOWN:
			if (press) ay = -0.5;
			else ay = 0;
			break;
		case 'G':
			if (press){
				gravity = ( gravity == 0.0) ? -0.098:0.0;
			}
			break;
		case 'z':
			if (press) zoom -= 0.03;
			break;
		case 'Z':
			if (press) zoom += 0.03;
			break;
		}
	}

	void draw(){
		//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, gold_amb);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, gold_diff);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, gold_spe);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, gold_shin);
		//glColor3f(r, g, b); // set drawing color to white
		glPushMatrix();
		glTranslatef(x,y,z);
		glutSolidSphere(radius, 20, 20);
		glPopMatrix();
	}
	void move(){
		ax = 0;
		ay = 0;
		az = gravity;
		vx += ax;
		vy += ay;
		vz += az;
		x += vx;
		y += vy;
		z += vz;
		//r = (vx*vx + vy*vy + vz*vz)/10;
	}
	void collision(){
		if ( ( x >100-radius && vx >0 ) || ( x < -100+radius && vx < 0 ) ){
			vx = -vx * 0.95;
		}
		if ( ( y >100-radius && vy >0 ) || ( y < -100+radius && vy < 0 ) ){
			vy = -vy * 0.95;
		}
		if ( ( z >100-radius && vz >0 ) || ( z < -100+radius && vz < 0 ) ){
			vz = -vz * 0.95;
			if ( z < -100 + radius ) z = -100 + radius;
		}
	}
	void collision(Ball *ball){
		if ( 4*radius*radius > ((this->x - ball->x)*(this->x - ball->x) + (this->y - ball->y)*(this->y - ball->y) + (this->z - ball->z)*(this->z - ball->z))){
			double cx = ball->x - x;
			double cy = ball->y - y;
			double cz = ball->z - z;
			double n = sqrt(cx*cx + cy*cy + cz*cz);
			cx = cx / n;
			cy = cy / n;
			cz = cz / n;
			double dot = (ball->vx - vx) * cx + (ball->vy - vy) * cy + (ball->vz - vz) * cz;
			if (dot > 0)return;
			vx += 0.98*dot*cx;
			vy += 0.98*dot*cy;
			vz += 0.98*dot*cz;
			ball->vx += -0.98*dot*cx;
			ball->vy += -0.98*dot*cy;
			ball->vz += -0.98*dot*cz;
		}
	}
};

class Ball *ball[10];


void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // sky color is light blue
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(250.0*zoom*cos(angle), 250.0*zoom*sin(angle), 250.0*zoom*cos(theta), 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	//draw plane
	glColor4d(0.0, 0.0, 0.0, 1);
	//glColor3d(0.0, 0.7, 0.0);
	/*
	glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glVertex3f(-100.0, -100.0, -100.0);
		glVertex3f(-100.0, 100.0, -100.0);
		glVertex3f(100.0, 100.0, -100.0);
		glVertex3f(100.0, -100.0, -100.0);
	glEnd();
	*/
	glColor3f(0.0, 0.0, 0.0);
	for (double x = -100; x <= 100; x += 50){
		glBegin(GL_LINE_LOOP);
		glVertex3d( x, 100, 100);
		glVertex3d( x,-100, 100);
		glVertex3d( x,-100,-100);
		glVertex3d( x, 100,-100);
		glEnd();
	}
	for (double y = -100; y <= 100; y += 50){
		glBegin(GL_LINE_LOOP);
		glVertex3d( 100, y, 100);
		glVertex3d( 100, y,-100);
		glVertex3d(-100, y,-100);
		glVertex3d(-100, y, 100);
		glEnd();
	}
	for (double z = -100; z <= 100; z += 50){
		glBegin(GL_LINE_LOOP);
		glVertex3d( 100, 100, z);
		glVertex3d(-100, 100, z);
		glVertex3d(-100,-100, z);
		glVertex3d( 100,-100, z);
		glEnd();
	}

	//draw ball
	for (int i = 0; i < max_ball; i++){
		ball[i]->draw();
	}

	glutSwapBuffers();
}


void idle(void)
{
	for (int i = 0; i < max_ball; i++){
		ball[i]->move();
		for (int j = 0; j < max_ball; j++){
			if (i != j) ball[i]->collision( ball[j] );
		}
		ball[i]->collision();
	}
	glutPostRedisplay(); // redisplay everything
}

void pressNormalKeys(unsigned char key, int xx, int yy)
{
	if (key == ESC || key == 'q' || key == 'Q') exit(0);
	ball[0]->keycontrol(key, true);
}

void pressSpecialKey(int key, int xx, int yy)
{
	ball[0]->keycontrol(key, true);
}

void releaseSpecialKey(int key, int x, int y)
{
	ball[0]->keycontrol(key,false);
}

void mouseMove(int x, int y)
{
	if (isDragging) { 
		angle = (xDragStart-x) * 0.005;
		theta = (y-yDragStart) * 0.002;
	}
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { // left mouse button pressed
			isDragging = 1; // start dragging
			if( xDragStart ==0 )xDragStart = x; // save x where button first pressed
			if (yDragStart == 0)xDragStart = y; // save y where button first pressed
		}
		else  { /* (state = GLUT_UP) */
			isDragging = 0; // no longer dragging
			xDragStart = 0;
			yDragStart = 0;
		}
	}
}

void reshape(int w, int h)
{
	float ratio = ((float)w) / ((float)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, ratio, 0.1, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);
}

int main(int argc, char **argv)
{
	double x, y, z;
	double r, g, b;
	for (int i = 0; i < max_ball; i++){
		x = rand() % 200 - 100;
		y = rand() % 200 - 100;
		z = rand() % 200 - 100;
		ball[i] = new Ball(5.0, x, y, z);
		r = rand() % 100 / 100.0;
		g = rand() % 100 / 100.0;
		b = rand() % 100 / 100.0;
		ball[i]->setColor(r, g, b);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(2500, 1500);
	glutCreateWindow("OpenGL3D sample");

	//LIGHT
	static GLfloat lightPosition[4] = { 0.0, 0.0, 100.0, 0.5 }; //光源の位置
	static GLfloat spotDirrection[3] = { 0.0, 0.0, -1.0 }; //スポットライトを向ける方向
	static GLfloat lightCol[3] = { 1.0, 1.0, 1.0 }; //拡散光
	static GLfloat lightAmbient[3] = { 0.25, 0.25, 0.25 }; //環境光
	static GLfloat lightSpecular[3] = { 1.0, 1.0, 1.0 }; //鏡面光


	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialCol);
	//glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirrection); //スポットライトの向ける方向（デフォルト (0,0,-1.0)）
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 150.0);// スポットライトの絞り（デフォルト 180.0）
	//glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.1);// スポットライトの中心からの減衰の度合い（デフォルト 0）
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//Fog
	//glFogi(GL_FOG_MODE, GL_LINEAR);
	//glFogi(GL_FOG_START, 2);
	//glFogi(GL_FOG_END, 100);
	//glEnable(GL_FOG);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//アルファの設定
	glEnable(GL_BLEND);//アルファのブレンド有効

	glEnable(GL_DEPTH_TEST);
	



	// register callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	//glutIgnoreKeyRepeat(1);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(pressNormalKeys);
	glutSpecialFunc(pressSpecialKey);
	glutSpecialUpFunc(releaseSpecialKey);

	glutMainLoop();

	return 0; // this is just to keep the compiler happy
}

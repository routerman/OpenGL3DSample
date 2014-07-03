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
class Player;

const int init_window_width=1200;
const int init_window_height=700;
const int field_x = 200;
const int field_y = 100;
const int goal_y = 20;
const int goal_z = 15;
const double gravity =  -0.098;
const int max_player = 10; //

class Ball *ball;
class Player *player[max_player]; //without keeper

class Obj{
protected:
public:
	double m, radius;
	double x, y, z;
	double vx, vy, vz;
	double ax, ay, az;
	double r, g, b;
	GLfloat mat_color[4];
   Obj(){}
	Obj(double x, double y, double z){
		this->x = x;
		this->y = y;
		this->z = z;
		vx = vy = vz = 0;
		ax = ay = az = 0;
	}
	//virtual void draw()=0;
	void setColor(double r, double g, double b){
		this->r = r;
		this->g = g;
		this->b = b;
		mat_color[0] = r;// 0.75164;
		mat_color[1] = g;// 0.60648;
		mat_color[2] = b;// 0.22648;
		mat_color[3] = 1.0;
	}
	void move(){
		az = gravity;
		vx += ax;
		vy += ay;
		vz += az;
		x += vx;
		y += vy;
		z += vz;

		vx *= 0.95;
		vy *= 0.95;
		vz *= 0.95;
	}
	void collision(){
		if ( ( x > field_x-radius && vx >0 ) || ( x < -field_x+radius && vx < 0 ) ){
			vx = -vx * 0.95;
		}
		if ( ( y > field_y-radius && vy >0 ) || ( y < -field_y+radius && vy < 0 ) ){
			vy = -vy * 0.95;
		}
		if ( z < radius && vz < 0 ){
			vz = -vz * 0.95;
			if ( z < radius ) z = radius;
		}
	}
};

class Ball: public Obj{
private:

public:
	Ball(double xx, double yy, double zz){
		//Obj::Obj(xx,yy,zz);
		this->x = xx;
		this->y = yy;
		this->z = 10;
		vx = vy = vz = 0;
		ax = ay = az = 0;
		radius = 3;
		setColor(1.0,1.0,1.0);
		m =5;
	}
	void draw(){
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_color);
		glPushMatrix();
		glTranslatef(x,y,z);
		glutSolidSphere(radius, 20, 20);
		glPopMatrix();
	}
	bool isGoal(){
		if( x>field_x && x>-field_x && y<goal_y && y>-goal_y && z<goal_z ) return true;
	}
	//drible
	void setBall(double x,double y,double z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	//kick
	void setVelocity(double vx,double vy,double vz){
		this->vx = vx;
		this->vy = vy;
		this->vz = vz;
	}
};


class Player : public Obj{
private:
	int id;
   int team;
	int mode;
	int camera;
	double accel, theta, ds_theta;
	double dx, dy, dz; //direction,dz=0;
	double px, py, pz; //postion(initial xyz)
public:
	Player(int team, double xx, double yy){
		//Obj::Obj(xx,yy,7);
		px = xx;
		py = yy;
		pz = 10;
		this->x = xx;
		this->y = yy;
		this->z = 10;
		vx = vy = vz = 0;
		ax = ay = az = 0;
		if(x>=0)theta = 3.14;
		else if(x<0)theta = 0.01;
		dx = cos(theta);
		dy = sin(theta);
		accel = ds_theta = dz =0;
		this->team = team;
		switch (team){
			case 0:setColor(0.7,0.0,0.0);break;
			case 1:setColor(0.0,0.0,0.7);break;
			case 2:setColor(1.0,1.0,1.0);break;
		}
		radius=4;
		m=100;
		mode=0;
		camera=0;
	}
	void move(){
		theta += ds_theta;
		dx = cos(theta);
		dy = sin(theta);
		ax = accel * dx;
		ay = accel * dy;
		Obj::move();
	}
	// collision between player and player, or player and ball
	void collision(Obj *obj){
		double diff = 4*radius*radius - ((this->x - obj->x)*(this->x - obj->x) + (this->y - obj->y)*(this->y - obj->y) + (this->z - obj->z)*(this->z - obj->z));
		if( diff > 0 ){
			double cx = obj->x - x;
			double cy = obj->y - y;
			double cz = obj->z - z;
			double n = sqrt(cx*cx + cy*cy + cz*cz);
			cx = cx / n;
			cy = cy / n;
			cz = cz / n;
			double dot = (obj->vx - vx) * cx + (obj->vy - vy) * cy + (obj->vz - vz) * cz;
			if (dot > 0)return; //
			double m1 = (0.98+diff/10)*dot * this->m / (this->m +obj->m);
			double m2 = (0.98+diff/10)*dot * obj->m  / (this->m +obj->m);
			vx += m2*cx;
			vy += m2*cy;
			vz += m2*cz;
			obj->vx += -m1*cx;
			obj->vy += -m1*cy;
			obj->vz += -m1*cz;
		}
	}
	void lookat(){
		if(camera == 0)gluLookAt( 0.0, -300.0, 200,   0.0, 0.0, 0.0,  0.0, 0.0, 1.0);
		else if(camera != 0)gluLookAt( x-50*dx, y-50*dy, 30,   x+dx, y+dy, 5.0,  0.0, 0.0, 1.0);
	}
	void draw(){
		//direction line
		GLfloat line_color[4] = { 0.0, 0.0, 0.0, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, line_color);
		glBegin(GL_LINES);
		glVertex3d( x, y, z);
		glVertex3d( x+8*dx, y+8*dy, z);
		glEnd();
		//Body
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_color);
		glPushMatrix();
		glTranslatef(x,y,z);
		glutSolidSphere(radius, 20, 20);
		glPopMatrix();
	}
	/*
	static int getNearestPlayerId(int team,double x,double y){
		double min=10000;
		for(int i=0;i<max_player;i++){
			if( player[i]->team == team ){
				
				//iif( >min )
			}
		}
	}
     */
	void control(Ball *ball){
		if( (x-ball->x)*(x-ball->x) + (y-ball->y)*(y-ball->y) <= 10000){
			//chase ball
			ds_theta = dx * ( ball->y - y ) - dy * ( ball->x - x );
			accel = 0.05;
		}else if( (x-ball->x)*(x-ball->x) + (y-ball->y)*(y-ball->y) <= 400){
			if( px>0 ) ds_theta = dx * -y - dy * ( -field_x - x );
			else ds_theta = dx * -y - dy * ( field_x - x );
			accel = 0.001;
			
			//if( (x+radius*dx-ball->x)*(x+radius*dx-ball->x) + (y+radius*dy-ball->y)*(y+radius*dy-ball->y) <= 4*ball->radius*radius){
				//drible
				//ball->setBall(x+(radius+ball->radius)*dx,y+(radius+ball->radius)*dy,3);
				//if( ds_theta>0.3 && ds_theta<-0.3 )
					//ball->setVelocity(5*dx,5*dy,2);
			//}
		}else{
			//back to each position
			ds_theta = dx * ( py - y ) - dy * ( px - x );
			accel = 0.05;
		}
		if(ds_theta >= 0.05)ds_theta=0.05;
		else if(ds_theta < -0.05)ds_theta=-0.05;
		if(accel >= 0.05)accel=0.05;
		else if(accel < -0.05)accel=-0.05;
	}

	void control(int key, bool press){
		switch (key) {
		case GLUT_KEY_RIGHT:
			if (press) ds_theta = -0.05;
			else ds_theta = 0.0;
			break;
		case GLUT_KEY_LEFT:
			if (press)  ds_theta = 0.05;
			else ds_theta = 0.0;
			break;
		case GLUT_KEY_UP:
			if (press) accel = 0.05;
			else accel = 0;
			break;
		case GLUT_KEY_DOWN:
			if (press) accel = -0.02;
			else accel = 0;
			break;
		case 'c':
			camera++;
			if(camera>2)camera=0;
			break;
		case 'z': //d
			if( (x+radius*dx-ball->x)*(x+radius*dx-ball->x) + (y+radius*dy-ball->y)*(y+radius*dy-ball->y) <= 4*ball->radius*radius)
				ball->setBall(x+(radius+ball->radius)*dx,y+(radius+ball->radius)*dy,3);
			break;
		case 'x': //kick
			if (press) {
				if( (x+radius*dx-ball->x)*(x+radius*dx-ball->x) + (y+radius*dy-ball->y)*(y+radius*dy-ball->y) <= 4*ball->radius*radius)
					ball->setVelocity(5*dx,5*dy,2);
			}
			break;
		}
	}
};

void display(void)
{
	glClearColor(0.7, 0.7, 0.7, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//set camera postion and direction;
	player[0]->lookat();
	
	//Draw field
	GLfloat field_color[4] = { 0.0, 0.7, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, field_color);
	glBegin(GL_QUADS);
	glNormal3d(0.0,0.0,1.0);
	glVertex3d( field_x, field_y, 0);
	glVertex3d(-field_x, field_y, 0);
	glVertex3d(-field_x,-field_y, 0);
	glVertex3d( field_x,-field_y, 0);
	glEnd();

	//Draw Lines
	GLfloat line_color[4] = { 0.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, line_color);
	for (double x = -field_x; x <= field_x; x += 40){
		glBegin(GL_LINES);
		glVertex3d( x, field_y, 0.2);
		glVertex3d( x,-field_y, 0.2);
		glEnd();
	}
	for (double y = -field_y; y <= field_y; y += 40){
		glBegin(GL_LINES);
		glVertex3d( field_x, y, 0.2);
		glVertex3d(-field_x, y, 0.2);
		glEnd();
	}
	//Draw Goal
	GLfloat goal_color[4] = { 1.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goal_color);
	glBegin(GL_QUADS);
	glNormal3d(-1.0,0.0,0.0);
	glVertex3d( field_x, goal_y, goal_z);
	glVertex3d( field_x,-goal_y, goal_z);
	glVertex3d( field_x,-goal_y,0);
	glVertex3d( field_x, goal_y,0);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3d(1.0,0.0,0.0);
	glVertex3d(-field_x, goal_y, goal_z);
	glVertex3d(-field_x,-goal_y, goal_z);
	glVertex3d(-field_x,-goal_y,0);
	glVertex3d(-field_x, goal_y,0);
	glEnd();

	//Draw ball
	ball->draw();
	//Draw players
	for (int i = 0; i < max_player; i++){
		player[i]->draw();
	}

	glutSwapBuffers();
}


void loop(void)
{
	//Control
	//player[0]->control();
	for (int i = 1; i < max_player; i++){
		player[i]->control(ball);
	}

	//Motion
	ball->move();
	for (int i = 0; i < max_player; i++){
		player[i]->move();
	}
	for (int i = 0; i < max_player; i++){
		player[i]->collision(ball);
		for (int j = 0; j < max_player; j++){
			if(i != j) player[i]->collision( player[j] );
		}
	}
	if( ball->isGoal() ){
		//need to detect goal before field
		if(ball->y > field_y)      std::cout <<0<<std::endl;
		else if(ball->y < -field_y)std::cout <<1<<std::endl;
	}
	ball->Obj::collision();
	for (int i = 0; i < max_player; i++){
		player[i]->Obj::collision();
	}
	//Draw
	glutPostRedisplay(); // redisplay everything;goto display function
}

void pressNormalKeys(unsigned char key, int xx, int yy){
	if (key == ESC){
		delete ball;
		delete[] *player;
		exit(0);
	}
	player[0]->control(key, true);
}

void pressSpecialKey(int key, int xx, int yy){
	player[0]->control(key, true);
}
void releaseSpecialKey(int key, int x, int y){
	player[0]->control(key,false);
}
void mouseMove(int x, int y){}
void mouseButton(int button, int state, int x, int y){}
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
	ball = new Ball( 0, 0, 100);
	player[ 0] = new Player(0,-50, 50);
	player[ 1] = new Player(1, 50, 50);
	player[ 2] = new Player(0,-50,-50);
	player[ 3] = new Player(1, 50,-50);
	player[ 4] = new Player(0,-100, 75);
	player[ 5] = new Player(1, 100, 75);
	player[ 6] = new Player(0,-100,-75);
	player[ 7] = new Player(1, 100,-75);
	player[ 8] = new Player(0,-150,  0);
	player[ 9] = new Player(1, 150,  0);

	//OpenGL initiation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(init_window_width,init_window_height);
	glutCreateWindow("soccer");

	//LIGHT
	static GLfloat lightPosition[4] = { 0.0, 0.0, 100.0, 0.5 };
	static GLfloat spotDirrection[3] = { 0.0, 0.0, -1.0 };
	static GLfloat lightCol[3] = { 1.0, 1.0, 1.0 };
	static GLfloat lightAmbient[3] = { 0.25, 0.25, 0.25 };
	static GLfloat lightSpecular[3] = { 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

// register callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(loop);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(pressNormalKeys);
	glutSpecialFunc(pressSpecialKey);
	glutSpecialUpFunc(releaseSpecialKey);

	glutMainLoop();

	delete ball;
	delete[] *player;
	return 0;
}

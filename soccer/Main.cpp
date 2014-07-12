#include "Include.hpp"
#include "Obj.hpp"
#include "Ball.hpp"
#include "Team.hpp"
#include "Player.hpp"

const int init_window_width=1300;
const int init_window_height=700;
const int field_x = 200;
const int field_y = 100;
const int goal_y = 20;
const int goal_z = 15;
const double gravity =  -0.098;
const int max_player = 11; //
const GLfloat lightPosition[4] = { 0.0, 0.0, 100.0, 0.5 };
//const	GLfloat spotDirrection[3] = { 0.0, 0.0, -1.0 };
const GLfloat lightCol[3] = { 1.0, 1.0, 1.0 };
//const GLfloat lightAmbient[3] = { 0.25, 0.25, 0.25 };
//const GLfloat lightSpecular[3] = { 1.0, 1.0, 1.0 };
const GLfloat field_color[4] = { 0.0, 0.7, 0.0, 1.0 };
const GLfloat line_color[4] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat goal_color[4] = { 1.0, 0.0, 0.0, 1.0 };

class Ball *ball;
class Team *team[2];
class Player *player[22];

void DrawString(int x,int y,const char *str){
	glMatrixMode(GL_PROJECTION); //GL_MODELVIEW(default) to GL_PROJECTION
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0,init_window_width,init_window_height,0);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			glRasterPos2d(x,y);
			while(*str){
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*str);
				str++;
			}

			glPopMatrix();
		glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW); //back to default

}

void display(void)
{
	glClearColor(0.7, 0.7, 0.7, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//set camera postion and direction;
	player[0]->lookat();
	
	//Draw field
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, field_color);
	glBegin(GL_QUADS);
	glNormal3d(0.0,0.0,1.0);
	glVertex3d( field_x, field_y, 0);
	glVertex3d(-field_x, field_y, 0);
	glVertex3d(-field_x,-field_y, 0);
	glVertex3d( field_x,-field_y, 0);
	glEnd();

	//Draw Lines
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, line_color);
	for (double x = -field_x; x <= field_x; x += 40){
		glBegin(GL_LINES);
		glVertex3d( x, field_y, 0.1);
		glVertex3d( x,-field_y, 0.1);
		glEnd();
	}
	for (double y = -field_y; y <= field_y; y += 40){
		glBegin(GL_LINES);
		glVertex3d( field_x, y, 0.1);
		glVertex3d(-field_x, y, 0.1);
		glEnd();
	}
	//Draw Goal
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
	for (int i = 0; i < 2*max_player; i++){
		player[i]->draw();
	}
	//DrawString(20,20," RED :");
	//DrawString(20,35,"BLUE :");
	DrawString(20,20,team[0]->name);
	DrawString(20,35,team[1]->name);
	char buf[5];
	sprintf(buf,"%d",team[0]->score);
	DrawString(80,20,buf);
	sprintf(buf,"%d",team[1]->score);
	DrawString(80,35,buf);

	glutSwapBuffers();
}



void loop(void)
{
	//Control
	team[0]->plan(ball,team[1]);
	team[1]->plan(ball,team[0]);
	//Motion
	ball->move();
	for (int i = 0; i < 2*max_player; i++){
		player[i]->move();
	}
	//collision with players
	for (int i = 0; i < 2*max_player; i++){
		player[i]->collision(ball);
		for (int j = 0; j < 2*max_player; j++){
			if( i != j ) player[i]->collision(player[j]);
		}
	}
	//need to detect goal before field
	int t=ball->isGoal();
	if( t == 0 || t == 1 ){
		team[t]->score++;
		ball->setBall(0,0,150);
	}
	ball->collision();
	for (int i = 0; i < 2*max_player; i++){
		player[i]->Obj::collision();
	}
	//Draw
	glutPostRedisplay(); // redisplay everything;goto display function
}

void pressNormalKeys(unsigned char key, int xx, int yy){
	if (key == ESC){
		delete ball;
		delete team[0];
		delete team[1];
		for(int i=0;i<2*max_player;i++){
			delete player[i];
		}
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
	team[0] = new Team(0,"RED");
	team[1] = new Team(1,"BLUE");
	player[ 0] = new Player(0,  50, 50);
	player[ 1] = new Player(0,  50, 30);
	player[ 2] = new Player(0,  50,-30);
	player[ 3] = new Player(0,  50,-50);
	player[ 4] = new Player(0, 100, 50);
	player[ 5] = new Player(0, 100, 20);
	player[ 6] = new Player(0, 100,-20);
	player[ 7] = new Player(0, 100,-50);
	player[ 8] = new Player(0, 150, 30);
	player[ 9] = new Player(0, 150,-30);
	player[10] = new Player(0, 195,  0);
	player[11] = new Player(1, -50, 50);
	player[12] = new Player(1, -50, 30);
	player[13] = new Player(1, -50,-30);
	player[14] = new Player(1, -50,-50);
	player[15] = new Player(1,-100, 50);
	player[16] = new Player(1,-100, 20);
	player[17] = new Player(1,-100,-20);
	player[18] = new Player(1,-100,-50);
	player[19] = new Player(1,-150, 30);
	player[20] = new Player(1,-150,-30);
	player[21] = new Player(1,-195,  0);

//OpenGL initiation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(init_window_width,init_window_height);
	glutCreateWindow("soccer");

	//LIGHT
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightCol);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
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
	delete team[0];
	delete team[1];
	//delete[] *player;
	for(int i=0;i<2*max_player;i++){
		delete player[i];
	}
	return 0;
}

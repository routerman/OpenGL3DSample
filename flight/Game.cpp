#include "Game.hpp"

Game::Game(int gametype){
	keystat=0;
	viewpoint=0;
	time_interval=10;
	this->type=gametype;
}

void Game::display(){
	//s->id=0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);	//3D有効化
    glPushMatrix();
	glTranslated(0,0,0);
	glRotatef(0.0, 0.0, 0.0, 1.0);
	list<Fighter>::iterator t = enemylist.begin();

	switch(viewpoint){
	case 0:
		eye   =fighter.r+1.5*fighter.u-6*fighter.d;
		centor=fighter.r+0.2*fighter.u;
		up    =fighter.u;
	break;
	case 1:
		eye   =fighter.r-3*fighter.d;
		centor=fighter.r-5*fighter.d;
		up    =fighter.u;
	break;
	case 2:
		eye   =t->r + 1.5*t->u - 6*t->d;
		centor=t->r+0.2*t->u;
		up    =t->u;
	break;
	case 3:
		eye.set(1000,1000,200);
		centor.set(0,0,0);
		up.set(0,0,1);
		break;
		default:
			break;
	}
	gluLookAt(eye.x,eye.y,eye.z,
			centor.x,centor.y,centor.z,
			up.x,up.y,up.z);

	glClearColor(0,0,0,0);

	//GRID
	glBegin(GL_LINES);
	glColor3f(1,1,1);//
	for(float x=-10000;x<=10000;x+=100){
		glVertex3f(x,-10000,0);
		glVertex3f(x, 10000,0);
	}
	for(float y=-10000;y<=10000;y+=100){
		glVertex3f(-10000,y,0);
		glVertex3f( 10000,y,0);
	}
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(0.2,0.2,0.2);//
	glVertex3f( 10000, 10000,-1);
	glVertex3f(-10000, 10000,-1);
	glVertex3f(-10000,-10000,-1);
	glVertex3f( 10000,-10000,-1);
	glEnd();

	/* xyz座標軸 */
	glBegin(GL_LINES);
	glColor3f(1,0,0);//
	glVertex3f(0,0,5);
	glVertex3f(20,0,5);
	glColor3f(0,1,0);//
	glVertex3f(0,0,5);
	glVertex3f(0,20,5);
	glColor3f(0,0,1);//
	glVertex3f(0,0, 5);
	glVertex3f(0,0,25);
	glEnd();

    glPushMatrix();
		glTranslated(-1000,3000,0);
		glColor3f(1,1,0);//
		glutSolidSphere(1000,6,6);
	glPopMatrix();
    glPushMatrix();
		glTranslated(100,-200,100);
		glColor3f(0,0,1);//
		glutSolidSphere(70,6,6);
	glPopMatrix();
    glPushMatrix();
		glTranslated(-200,-300,40);
		glColor3f(1,0,0);//
		glutSolidSphere(50,6,6);
	glPopMatrix();
    glPushMatrix();
		glTranslated(0,0,300);
		glColor3f(0,1,1);//
		glutSolidSphere(200,6,6);
	glPopMatrix();
    glPushMatrix();
		glTranslated(200,400,200);
		glColor3f(1,0,0);//
		glutSolidSphere(100,6,6);
	glPopMatrix();

    glPushMatrix();
		glTranslated(0,0,5000);
		glColor3f(1,1,0);//
		glutSolidSphere(100,6,6);
	glPopMatrix();


	glColor3f(1,0,0);//
	fighter.drow();
	for( list<Fighter>::iterator it = enemylist.begin(); it != enemylist.end(); it++ ){
		it->drow();
	}
	for( list<Bullet>::iterator it = bulletlist.begin(); it != bulletlist.end(); it++ ){
		it->drow();
	}
	for( list<Bomb>::iterator it = bomblist.begin(); it != bomblist.end(); it++ ){
		it->drow();
	}
	for( list<Surface>::iterator it = surfacelist.begin(); it != surfacelist.end(); it++ ){
		it->drow();
	}
    //glDisable(GL_POINT_SMOOTH);
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glutSwapBuffers(); 
};


void Game::timer(){
	fighter.model();
	if( keystat & KEYSTAT_BOMB ) fighter.dropbomb(&bomblist);
	if( keystat & KEYSTAT_SHOOT ) fighter.shoot(&bulletlist);
	for( list<Fighter>::iterator it = enemylist.begin(); it != enemylist.end(); it++ ){
		it->model();
		it->dropbomb(&bomblist);
		it->shoot(&bulletlist);
	}
	for( list<Bullet>::iterator it = bulletlist.begin(); it != bulletlist.end(); it++ ){
		it->move();
	}
	for( list<Bomb>::iterator it = bomblist.begin(); it != bomblist.end(); it++ ){
		it->model();
	}
}

void Game::init(){
	fighter.r.set(100,0,10);
	fighter.d.set(1,0,0);
	Fighter a;
	a.r.set(0,0,10);
	a.d.set(1,0,0);
	enemylist.push_back(a);
	a.r.set(0,0,100);
	a.d.set(1,0,0);
	enemylist.push_back(a);
	a.r.set(0,0,1000);
	a.d.set(1,0,0);
	enemylist.push_back(a);
	Bomb b;
	b.r.set(0,0,0);
	b.v.set(0,0,0);
	b.a.set(0,0,0);
	bomblist.push_back(b);

	//Surface initialization
	Surface s;
	F3 s1,s2,s3,s4;
}

void Game::keydown(int key){
	switch (key) {
		case GLUT_KEY_UP:	//前進
		//case '8':
			keystat|=KEYSTAT_GO;
			break;
		case GLUT_KEY_DOWN:	//後進
		//case '5':	
			keystat|=KEYSTAT_BACK;
			break;
		case GLUT_KEY_LEFT:	//左旋回
		//case '4':
			keystat|=KEYSTAT_LEFT;
			break;
		case GLUT_KEY_RIGHT:	//右旋回
		//case '6':	
			keystat|=KEYSTAT_RIGHT;
			break;
		case 'z':	//ジャンプ
			keystat|=KEYSTAT_BOOST;
			break;
		case KEY_SHOOT:	//ショット
			keystat|=KEYSTAT_SHOOT;
			break;
		case KEY_BOMB:	//ショット
			keystat|=KEYSTAT_BOMB;
			break;
		case KEY_CHANGE_VIEWPOINT:	//視点切り替え
			keystat |= KEYSTAT_CVIEW;
			if( ~keystat & KEYSTAT_FLAG1 ){
				viewpoint++;
				if(viewpoint>2)viewpoint=0;
				keystat |= KEYSTAT_FLAG1;	//フラグを立てる。
			}
			break;
		/*
		case KEY_CHANGE_WEAPON:	//視点切り替え
			keystat |= STAT_CWEAP;
			break;
			*/
		case KEY_RESET:
			init();
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}

void Game::keyup(int key){
	switch (key){
		case GLUT_KEY_UP:	//前進
		//case '8':	
			keystat &= ~KEYSTAT_GO;
			break;
		case GLUT_KEY_DOWN:	//後進
		//case '5':	
			keystat &= ~KEYSTAT_BACK;
			break;
		case GLUT_KEY_LEFT:	//左旋回
		//case '4':
			keystat &= ~KEYSTAT_LEFT;
			break;
		case GLUT_KEY_RIGHT:	//右旋回
		//case '6':	
			keystat &= ~KEYSTAT_RIGHT;
			break;
		case 'z':	//ジャンプ
			keystat &= ~KEYSTAT_BOOST;
			break;
		case KEY_SHOOT:	//ショット
			keystat &= ~KEYSTAT_SHOOT;
			break;
		case KEY_BOMB:	//ショット
			keystat &= ~KEYSTAT_BOMB;
			break;
		case KEY_CHANGE_VIEWPOINT:	//視点切り替え
			keystat &= ~KEYSTAT_CVIEW;
			keystat &= ~KEYSTAT_FLAG1;	//フラグリセット
			break;
			/*
		case KEY_CHANGE_WEAPON:	//武器切り替え
			keystat &= ~STAT_CWEAP;
			keystat &= ~STAT_FLAG2;	//フラグリセット
			break;
			*/
		default:
			break;
	}
}

void Game::control(){
	fighter.control(keystat);
	for( list<Fighter>::iterator it = enemylist.begin(); it != enemylist.end(); it++ ){
		it->drive(fighter);
	}
}

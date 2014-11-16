
#include"Bullet.hpp"

void Bullet::fire(F3 r,F3 d,F3 v){
	this->r=r+3*d;
	this->d=d;
	this->v=v+5*d;
	this->active=true;
}

void Bullet::move(){
	r=r+v;
	if( F3::range(r)>10000)active=false;
}

void Bullet::drow(){
    //glPushMatrix();
	//glTranslated(r.x,r.y,r.z);
	glColor3f(1,0,0);
	//glutSolidSphere(1,6,6);
	glBegin(GL_LINES);
		glLineWidth(5);
		glColor3f(1,1,0);
		glVertex3f(r.x,r.y,r.z);
		glColor3f(1,0,0);
		glVertex3f(r.x+d.x,r.y+d.y,r.z+d.z);
	glEnd();
	//glPopMatrix();
}

Bullet::Bullet(){
	active=false;
}

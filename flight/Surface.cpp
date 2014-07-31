#include "Surface.hpp"

Surface::Surface(){
	this->color.set(0,0,0);	
}


Surface::~Surface(){
	
}


void Surface::set(F3 a1,F3 a2,F3 a3,F3 a4){
	this->s1=a1;
	this->s2=a2;
	this->s3=a3;
	this->s4=a4;
	this->n=F3::cross(a2-a1,a4-a1);
}


void Surface::drow(){
	glBegin(GL_QUADS);
	glColor3f(color.x,color.y,color.z);
	glVertex3f(s1.x,s1.y,s1.z);
	glVertex3f(s2.x,s2.y,s2.z);
	glVertex3f(s3.x,s3.y,s3.z);
	glVertex3f(s4.x,s4.y,s4.z);
	glEnd();
}
#include"Bomb.hpp"



Bomb::Bomb(){
	state=NOUSE;
	time=0;
	r.set(0,0,0);
	v.set(0,0,0);
	a.set(0,0,0);
}

Bomb::~Bomb(){

}


void Bomb::fire(F3 r,F3 d,F3 v){
	time=0;
	state=MOVE;
	this->r=r+3*d;
	this->d=d;
	this->v=v+3*d;
	this->active=true;
}


void Bomb::model(){
	move();
	time++;
	if(F3::range(r)>10000){
		state=NOUSE;
		//~Bomb();
	}
	if(r.z<0 && state==MOVE){
		state=FIRE;
		time=0;
		v.set(0,0,0);
	}
	
	/*
	if( time>500 && state==MOVE){
		state=FIRE;
		time=0;
		v.set(0,0,0);
	}else if(time>1000 && state==FIRE){
		state=NOUSE;
		//~Bomb();
	}
	*/
}

void Bomb::move(){
	r+=v;
}

void Bomb::drow(){
	switch(state){
	case MOVE:
		glPushMatrix();
		glTranslated(r.x,r.y,r.z);
		glColor3f(0.7,0.7,0.7);
		glutSolidSphere(4,12,12);
		glPopMatrix();
		glEnd();
		break;
	case FIRE:
		glPushMatrix();
		glTranslated(r.x,r.y,r.z);
		glColor3f(1,1-(float)time/1000,0);
		if(time>0 && time<=500 )glutSolidSphere(1000*(1-exp(-(float)time/50)),12,12);
		else if(time>500 && time<1000)glutSolidSphere(1000*(1-exp((float)(time-1000)/50)),12,12);
		glPopMatrix();
		break; 
	}
}

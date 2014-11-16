#include"Bomb.hpp"



Bomb::Bomb(){
	state=NOUSE;
	time=0;
	radius = 4;
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
	this->v=v+2*d;
	this->active=true;
}

WeaonState Bomb::getState(){
	return state;
}

void Bomb::model( list<Fighter> *enemys ) {
	move();
	time++;
	if ( ( state == NOUSE ) || ( state == FIRE && time > 400 ) ) {
		state = NOUSE;
		return;
	} else if ( state == MOVE ) {
		if ( time == 500 || r.x > HORIZON || r.x < -HORIZON 
							  || r.y > HORIZON || r.y < -HORIZON 
							  || r.z > HORIZON || r.z < -HORIZON ) {
			state = FIRE;
			time = 0;
			v.set(0,0,0);
			return;
		}
		for( list<Fighter>::iterator it = enemys->begin(); it != enemys->end(); it++ ){
			if ( F3::range( it->r - this->r ) < radius ) {
				state=FIRE;
				time=0;
				v.set(0,0,0);
				return;
			}
		}
	} else if (state == FIRE ) {
		for( list<Fighter>::iterator it = enemys->begin(); it != enemys->end(); it++ ){
			if ( F3::range( it->r - this->r ) < radius ) {
					it->decreaseHP(1);
			}
		}
	}
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
		glutSolidSphere(radius,12,12);
		glPopMatrix();
		glEnd();
		break;
	case FIRE:
		glPushMatrix();
		glTranslated(r.x,r.y,r.z);
		glColor3f(1,1-(float)time/400,0);
		if(time>0 && time<=200 ) radius = 400 * ( 1 - exp( -(float)time/20) );
		else if(time>200 && time<400) radius = 400 * (1 - exp( (float)(time-400)/20) );
		glutSolidSphere( radius, 12, 12);
		glPopMatrix();
		break; 
	}
}

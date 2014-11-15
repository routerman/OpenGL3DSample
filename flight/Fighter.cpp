#include "Fighter.hpp"

Fighter::Fighter(){
	r.set(0,20,3);
	v.set(0.1,0,0);
	a.set(0,0,0);
	u.set(0,0,1);
	d.set(1,0,0);
	m=1000;
	size=3;
	roll=0;
	pitch=0;
	f=0;
}

//MODEL

void Fighter::model(){
	move();
}

void Fighter::move(){
	//roll
	u=F3::rot(u,d,roll);
	//pitch
	u=F3::rot(u, F3::unit(F3::cross(d,u)),pitch);
	d=F3::rot(d, F3::unit(F3::cross(d,u)),pitch);
	v=F3::range(v)*d;
	a=f/m*d-0.1*pitch*v;
	
	v+=a;
	r+=v;
}

void Fighter::shoot( list<Bullet> *bullets){
	if ( --bulletwait < 0 && command == 1) {
		cout << " shoot!" << endl;
		Bullet b;
		b.fire(r,d,v);
		bullets->push_back(b);
		bulletwait=10;
	}
}

void Fighter::dropbomb( list<Bomb> *bombs){
	if ( --bombwait > 0 || command != 2) return;
	cout << " Bomb!" << endl;
	//Bomb b;
	//b.fire(r,d,v);
	//bombs->push_back(b);
	bombwait=10;
}


//VIEW
void Fighter::drow(){
	glPushMatrix();
	glTranslatef(r.x,r.y,r.z);	//‹@‘Ì‚ÌˆÊ’u
	F3 a = F3::cross(u,d);

	glRotatef(-atan2f(u.y,u.z)*180/M_PI,1,0,0);//xŽ²
	glRotatef(		asinf(u.x)*180/M_PI,0,1,0);//yŽ²
	glRotatef(-atan2f(a.x,d.x)*180/M_PI,0,0,1);//zŽ²

	GLfloat v[5][3]={
			{size/2,0,size/3},
			{-size,size,0},
			{-size,-size,0},
			{size,0,0},
			{-size,size,0},
	};
	glBegin(GL_TRIANGLE_FAN);
		glColor3f(0,0,1);
		glVertex3fv(v[0]);
		glColor3f(1,0,1);
		glVertex3fv(v[1]);
		glColor3f(1,0,0);
		glVertex3fv(v[2]);
		glColor3f(0,0,1);
		glVertex3fv(v[3]);
		glColor3f(0,0,1);
		glVertex3fv(v[4]);
	glEnd();
	glPopMatrix();
}


//CONTROL
void Fighter::control(int keystat){
	//if(!(players[id].mode & MODE_DEAD)){
	if( keystat & KEYSTAT_GO )pitch=-0.02;
	else if( keystat & KEYSTAT_BACK )pitch=0.02;
	else pitch=0;

	if( keystat & KEYSTAT_LEFT )roll=-0.03;
	else if( keystat & KEYSTAT_RIGHT )roll=0.03;
	else roll=0;

	//boost
	if( keystat & KEYSTAT_BOOST )f=10;
	//else if( keystat & STAT_CWEAP )f=0.1;
	else f=0;

	//shot
	command = 0;
	if( keystat & KEYSTAT_SHOOT )command = 1;
	if( keystat & KEYSTAT_BOMB )command = 2;

	if( (keystat & ~KEYSTAT_IDLE)==0 ){
		keystat=KEYSTAT_IDLE;
		f=0;
	}
	
}

void Fighter::drive(Fighter target){
	F3 w, uxd, b;
	w = target.r - this->r;
	float dist = F3::range(w);
	if ( dist > 1000 ) {
		f = 10;
		c = 0;
	} else if( dist <= 1000 && dist > 300 ){
		f = 3;
		c = 0;
	} else {
		f = 0.5;
		c = 0.1;
	}
	w = F3::unit(w);
	uxd = F3::unit( F3::cross( this->u, this->d ) );
	b = F3::cross( this->u, w);
	pitch = roll = 0;

	roll = -( w * uxd );
	pitch= w*u;
	if( w*d < 0 ) pitch=0.01;

	float cc = cosf( 2 * atanf( pitch / ( roll + 0.001 ) ) );
	//if(cc<0)cc*=-1;
	cc*=cc;
	pitch*=cc;
	//if( roll * roll + pitch * pitch < 0.01 && F3::dot(w,this->d) > 0){
	if( roll * roll + pitch * pitch < 0.0001 && dist < 100 ){
		command = 1;
	}

	//regulater
	if( roll > 0.05 ) roll = 0.05;
	else if( roll < -0.05 ) roll = -0.05;
	else roll = roll - 5 * roll * roll;
	if( pitch > 0.01 ) pitch = 0.01;
	else if( pitch < -0.01 ) pitch = -0.01;
	else pitch = pitch - 5 * pitch * pitch;
}

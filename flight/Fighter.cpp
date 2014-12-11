#include "Fighter.hpp"

Fighter::Fighter(){
	name = "aaa";
	m=1000;
	size=3;
	roll=0;
	pitch=0;
	f=0;
	max_f=20;
	init();
}

void Fighter::init(){
	r.set(0,-1000,1000);
	v.set(0,10,0);
	a.set(0,0,0);
	u.set(0,0,1);
	d.set(0,1,0);
	bulletwait=0;
	bombwait=0;
	hp=1000;
}
//MODEL

void Fighter::decreaseHP(unsigned int damage){
	hp -= damage;
	v.set(0,0,10); //vibration
}

void Fighter::model(){
	if ( hp < 0
		|| r.x < -HORIZON || r.x > HORIZON 
		|| r.y < -HORIZON || r.y > HORIZON 
		|| r.z < -HORIZON || r.z > HORIZON ) {
		init();
	}
	move();
}

void Fighter::move(){
	//roll
	u=F3::rot(u,d,roll);
	//pitch
	u=F3::rot(u, F3::unit(F3::cross(d,u)),pitch);
	d=F3::rot(d, F3::unit(F3::cross(d,u)),pitch);
	v= 0.3*F3::range(v)*d + 0.7*v;
	a=f/m*d-0.1*pitch*v;
	a.z-=GRAVITY/100; //10ms = 1/100s
	cout << a.z <<endl;
	v+=a;
	r+=v;
}

void Fighter::shoot( list<Bullet> *bullets){
	bulletwait--;
	//cout << bulletwait << endl;
	if ( bulletwait < 0 && command == 1) {
	 	//cout << " shoot!" << endl;
 		Bullet b;
		b.fire(r,d,v);
		bullets->push_back(b);
		bulletwait=50;
		command = 0;
	}
}

void Fighter::dropbomb( list<Bomb> *bombs){
	bombwait--;
	if ( bombwait < 0 && command == 2) {
		//cout << " Bomb!" << endl;
		Bomb b;
		b.fire(r,d,v);
		bombs->push_back(b);
		bombwait=200;
		command = 0;
	}
}


//VIEW
void Fighter::drow(){
	glPushMatrix();
	glTranslatef(r.x,r.y,r.z);	//ã@ëÃÇÃà íu
	F3 a = F3::cross(u,d);

	glRotatef(-atan2f(u.y,u.z)*180/M_PI,1,0,0);//xé≤
	glRotatef(		asinf(u.x)*180/M_PI,0,1,0);//yé≤
	glRotatef(-atan2f(a.x,d.x)*180/M_PI,0,0,1);//zé≤

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
	if( keystat & KEYSTAT_BOOST )f=max_f;
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

Fighter* Fighter::getTarget( list<Fighter> *enemys ){
	double dist, min_dist = HORIZON;
	F3 tmp;
	Fighter *target;
	list<Fighter>::iterator it = enemys->begin();
	target = &(*it);
	for( list<Fighter>::iterator it = enemys->begin(); it != enemys->end(); it++ ){
		tmp = it->r - this->r;
		dist = F3::range(tmp);
		if ( dist > 0.1 && dist < min_dist ){
			min_dist = dist;
			target = &(*it);
		}
	}
	return target;
}

void Fighter::drive( list<Fighter> *enemys ){
	F3 w, uxd, b;
	Fighter *target = getTarget(enemys);

	w = target->r - this->r;
	double dist = F3::range(w);
	//cout << target->name << dist << endl;
	if ( dist > 1000 ) {
		f = max_f;
		c = 0;
	} else if( dist <= 1000 && dist > 300 ){
		f = max_f/5;
		c = 0;
	} else {
		f = 0;
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
	if( roll * roll + pitch * pitch < 0.05 ){
		if ( dist > 0 && dist < 1000 ) {
			command = 1; //shoot
		//} else if ( dist < 300 ) {
			command = 2; //bomb
			f = 0;
		}
	}

	//regulater
	if( roll > 0.05 ) roll = 0.05;
	else if( roll < -0.05 ) roll = -0.05;
	else roll = roll - 5 * roll * roll;
	if( pitch > 0.05 ) pitch = 0.05;
	else if( pitch < -0.05 ) pitch = -0.05;
	else pitch = pitch - 5 * pitch * pitch;
	if ( roll > 0.05 || roll < -0.05 ) pitch = 0;
}

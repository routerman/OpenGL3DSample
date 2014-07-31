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

void Fighter::dropbomb( list<Bomb> *a){
	Bomb b;
	b.fire(r,d,v);
	a->push_back(b);
	bombwait=50;
}


//VIEW
void Fighter::drow(){
	glPushMatrix();
	glTranslatef(r.x,r.y,r.z);	//�@�̂̈ʒu
	F3 a = F3::cross(u,d);
	/* ��]�s��@XYZ�̏��Ԃɉ�]���� */
	glRotatef(-atan2f(u.y,u.z)*180/M_PI,1,0,0);//x��
	glRotatef(		asinf(u.x)*180/M_PI,0,1,0);//y��
	glRotatef(-atan2f(a.x,d.x)*180/M_PI,0,0,1);//z��

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
	//�O�i����i��
	//if(!(players[id].mode & MODE_DEAD)){
	if( keystat & KEYSTAT_GO )pitch=-0.02;
	else if( keystat & KEYSTAT_BACK )pitch=0.02;
	else pitch=0;

	//�����E��
	if( keystat & KEYSTAT_LEFT )roll=-0.03;
	else if( keystat & KEYSTAT_RIGHT )roll=0.03;
	else roll=0;

	//boost
	if( keystat & KEYSTAT_BOOST )f=10;
	//else if( keystat & STAT_CWEAP )f=0.1;
	else f=0;

	//shot
	//if( keystat & KEYSTAT_SHOT )comand=1;

	//�X�y�[�X�ŕ���̃^�C�v��؂�ւ���
	//���������ĂȂ��Ƃ�
	if( (keystat & ~KEYSTAT_IDLE)==0 ){
		keystat=KEYSTAT_IDLE;
		f=0;
	}
	
}
void Fighter::drive(Fighter target){
	F3 w,uxd,b;
	w=target.r-this->r;
	float dist=F3::range(w);
	if(dist>300){
		f=1;
		c=0;
	}else if(dist<=300 && dist>30){
		f=0.7;
		c=0;
	}else {
		f=0.5;
		c=0.1;
	}
	w=F3::unit(w);
	uxd = F3::unit( F3::cross(this->u,this->d) );
	b = F3::cross(this->u,w);
	pitch=roll=0;

	roll=-(w*uxd);
	pitch= w*u;
	if( w*d<0 )pitch=0.01;//�@�̌���ɂ���Ƃ��A

	float cc=cosf(2*atanf(pitch/(roll+0.001)));
	//if(cc<0)cc*=-1;
	cc*=cc;
	pitch*=cc;
	//roll-=uxd.z;
	//if( uxd.z>0.1 || uxd.z<-0.1 )roll*=uxd.z;
	/*
	//if(roll>-0.01 && roll<0.01){
		//roll*=0.1;
		pitch=0.1 * F3::dot( F3::cross(w,this->d),uxd);
		if(roll*roll+pitch*pitch<0.01 && F3::dot(w,this->d) > 0){
			//min{-u.z}(roll)
			//if(pitch>-0.01 && pitch < 0.01)roll+=0.01*u.y;
			//if(dist<100);
		}
	//}
	*/
	//regulater
	if(roll>0.01)roll=0.01;
	else if(roll<-0.01)roll=-0.01;
	if(pitch>0.01)pitch=0.01;
	else if(pitch<-0.01)pitch=-0.01;

}

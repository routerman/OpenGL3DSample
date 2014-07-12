#pragma once
#include "Include.hpp"
#include "Obj.hpp"

class Ball: public Obj{
private:

public:
	Ball(double xx, double yy, double zz){
		//Obj::Obj(xx,yy,zz);
		r.set(xx,yy,10);
		v.set(0.0,0.0,0.0);
		a.set(0.0,0.0,0.0);
		setColor(1.0,1.0,1.0);
		radius = 3;
		m =0.6;
	}
	void draw(){
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_color);
		glPushMatrix();
		glTranslatef(r.x,r.y,r.z);
		glutSolidSphere(radius, 20, 20);
		glPopMatrix();
	}
	int isGoal(){
		if( r.y<goal_y && r.y>-goal_y && r.z<goal_z ){
			if( r.x<-field_x+radius )return 0;
			else if( r.x>field_x-radius )return 1;
		}
		return -1;
	}
	//dribble
	void setBall(double x,double y,double z){
		this->r.set(x,y,z);
	}
	//kick
	void setVelocity(double vx,double vy,double vz){
		this->v.set(vx,vy,vz);
	}
};

#pragma once
#include "Include.hpp"
#include "Vector3D.hpp"
#include "Team.hpp"
#include "Obj.hpp"

//extern class Team;
enum Mode{WAIT,CHASE,TURN,DRIBBLE,KICK,BACK};


class Player : public Obj{

private:
	int id;
	//Team& team;
	int team_id;
	Mode mode;
	int camera;
	double accel, theta, ds_theta;
	Vector3D d;
	Vector3D p;
	Vector3D goal;
public:
	Player(int team, double xx, double yy){
		//Obj::Obj(xx,yy,7);
		this->team_id = team;
		if(xx>0)p.set(-2*( field_x-xx)+field_x, yy*1.4, 0.0);
		else    p.set(-2*(-field_x-xx)-field_x, yy*1.4, 0.0);
		r.set(xx,yy,10);
		v.set(0.0,0.0,0.0);
		a.set(0.0,0.0,0.0);
		if(r.x>=0) goal.set( -field_x, 0.0, 0.0);
		else if(r.x<0) goal.set( field_x, 0.0, 0.0);
		if(r.x>=0)theta = 3.14;
		else if(r.x<0)theta = 0.01;
		d.set( cos(theta), sin(theta), 0.0);
		accel = ds_theta =0;
		
		switch (this->team_id){
			case 0:setColor(0.7,0.0,0.0);break;
			case 1:setColor(0.0,0.0,0.7);break;
			case 2:setColor(1.0,1.0,1.0);break;
		}
		
		radius=4;
		m=100;
		mode=WAIT;
		camera=0;
	}
	~Player(){}
	void setMode(Mode mode){
		this->mode=mode;
	}
	void setDirection(Obj *obj){
		this->d = Vector3D::normalize( obj->r-this->r );
	}
	int getTeamId(){
		return this->team_id;
	}
	void move(){
		theta += ds_theta;
		d.set( cos(theta), sin(theta), 0.0);
		a = accel * d;
		Obj::move();
		double dd = (v.x*d.y-d.x*v.y) / v.getNorm();
		v.x *= (19+dd)/20;
		v.y *= (19+dd)/20;
	}
	// collision between player and player, or player and ball
	void collision(Obj *obj){
		double diff = (this->radius+obj->radius) - Vector3D::getDistance(this->r,obj->r);
		if( diff > 0 ){
			Vector3D rr = Vector3D::normalize(obj->r - this->r);
			double impact_speed = Vector3D::inner_product( obj->v - this->v, rr);
			if ( impact_speed < 0 ){
				obj->v  +=  ( (0.99+diff*diff) * this->m / (this->m+obj->m) ) * -impact_speed * rr;
				this->v +=  ( (0.99+diff*diff) * obj->m  / (this->m+obj->m) ) *  impact_speed * rr;
			}
		}
	}
	void lookat(){
		if(camera == 0)gluLookAt( 0.0, -50.0, 300,   0.0, 0.0, 0.0,  0.0, 0.0, 1.0);
		else if(camera != 0)gluLookAt( r.x-50*d.x, r.y-50*d.y, 30,   r.x+d.x, r.y+d.y, 5.0,  0.0, 0.0, 1.0);
	}
	void draw(){
		//direction line
		GLfloat line_color[4] = { 0.0, 0.0, 0.0, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, line_color);
		glBegin(GL_LINES);
		glVertex3d( r.x, r.y, r.z);
		glVertex3d( r.x+8*d.x, r.y+8*d.y, r.z);
		glEnd();
		//Body
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_color);
		glPushMatrix();
		glTranslatef(r.x,r.y,r.z);
		glutSolidSphere(radius, 20, 20);
		glPopMatrix();
	}
	//control for AI
	void control(Ball *ball){
		//enum Mode{WAIT,CHASE,TURN,DRIBBLE,BACK};
		if( mode == WAIT ){
			mode = WAIT;
		}else if( mode == CHASE ){
			if( Vector3D::getDistance( r+radius*d, ball->r) <= 2*radius ){
				if( cos(  Vector3D::inner_product(d,goal-r) / ( Vector3D::getNorm(d) * Vector3D::getNorm(goal-r) )  ) > 0.9 ){
					mode = DRIBBLE;
				}else{
					mode = TURN;
				}
				int count=0;
				for (int i = 0; i < 2*max_player; i++){
					if( ( player[i]->getTeamId() != this->team_id ) && ( Vector3D::getDistance( player[i]->r, this->r+3*radius*this->d ) < 4*radius ) ){
						count++;
						if(count>=2){
							mode = KICK;
							break;
						}
					}
				}
			}else{
				mode = CHASE;
			}
		}else if( mode == BACK ){
			mode = BACK;
			if( Vector3D::getDistance(p,r) < 20 ){
				mode = WAIT;
			}
		}else{
			mode = BACK;
		}

		//Plan
		if( mode==WAIT ){
			//wait at position
			ds_theta = Vector3D::getAngle(d, ball->r-r);
			accel = 0.0;
		}else if( mode==CHASE ){
			//chase ball
			ds_theta = Vector3D::getAngle(d, ball->r-r);
			accel = 0.5;
		}else if( mode==TURN ){
			//set direction to goal
			ds_theta = Vector3D::getAngle(d, goal-r);
			accel = -0.1;
			//set ball velocity to turn
			double n = Vector3D::getDistance(ball->r,this->r);
			double phi = 0.01*Vector3D::getAngle(goal-r,ball->r-r);
			if( phi>1)phi=1;
			else if( phi<-1)phi=-1;
			ball->setVelocity(  phi*(ball->r.y-r.y)/n+0.5*d.x, -phi*(ball->r.x-r.x)/n+0.5*d.y,0);
		}else if( mode==DRIBBLE ){
			ds_theta = Vector3D::getAngle(d, goal-r);
			accel = 0.5;
			//set ball velocity to dribble
			ball->setVelocity( 2*v.x,2*v.y,0);
		}else if( mode==KICK ){
			// if there are enemyies aroung
			ball->setVelocity( 3*d.x,3*d.y,2);
			mode = BACK;
		}else if( mode==BACK ){
			//back to each position
			ds_theta = Vector3D::getAngle(d, p-r);
			accel = 0.5;
		}
		
		//Limmiter
		if(ds_theta >= 0.05)ds_theta=0.05;
		else if(ds_theta < -0.05)ds_theta=-0.05;
		if(accel >= 0.05)accel=0.05;
		else if(accel < -0.05)accel=-0.05;
	}

	//control for Human
	void control(int key, bool press){
		switch (key) {
		case GLUT_KEY_RIGHT:
			if (press) ds_theta = -0.05;
			else ds_theta = 0.0;
			break;
		case GLUT_KEY_LEFT:
			if (press)  ds_theta = 0.05;
			else ds_theta = 0.0;
			break;
		case GLUT_KEY_UP:
			if (press) accel = 0.05;
			else accel = 0;
			break;
		case GLUT_KEY_DOWN:
			if (press) accel = -0.02;
			else accel = 0;
			break;
		case 'c':
			camera++;
			if(camera>2)camera=0;
			break;
		case 'z': //d
			//r + radius*d - ball->r;
			if( (r.x+radius*d.x-ball->r.x)*(r.x+radius*d.x-ball->r.x) + (r.y+radius*d.y-ball->r.y)*(r.y+radius*d.y-ball->r.y) <= 4*ball->radius*radius)
				ball->setBall(r.x+(radius+ball->radius)*d.x,r.y+(radius+ball->radius)*d.y,3);
			break;
		case 'x': //kick
			if (press) {
				if( (r.x+radius*d.x-ball->r.x)*(r.x+radius*d.x-ball->r.x) + (r.y+radius*d.y-ball->r.y)*(r.y+radius*d.y-ball->r.y) <= 4*ball->radius*radius)
					ball->setVelocity(5*d.x,5*d.y,2);
			}
			break;
		}
	}
};



#pragma once
#include "Include.hpp"
#include "Vector3D.hpp"
class Obj{
protected:
public:
	double m, radius;

	Vector3D r;
	Vector3D v;
	Vector3D a;
	double red, green, blue;
	GLfloat mat_color[4];
   Obj(){}
	Obj(double x, double y, double z){
		r.set(x,y,z);
		v.set(0.0,0.0,0.0);
		a.set(0.0,0.0,0.0);
	}
	//virtual void draw()=0;
	void setColor(double red, double green, double blue){
		this->red = red;
		this->green = green;
		this->blue = blue;
		mat_color[0] = red;// 0.75164;
		mat_color[1] = green;// 0.60648;
		mat_color[2] = blue;// 0.22648;
		mat_color[3] = 1.0;
	}
	double getDistance(Obj *obj){
		return Vector3D::getDistance(this->r,obj->r);
	}
	void move(){
		a.z = gravity;
		v+=a;
		r+=v;
		v.x *= 0.95;
		v.y *= 0.95;
		v.z *= 0.95;
	}
	void collision(){
		if ( ( r.x > field_x-radius && v.x >0 ) || ( r.x < -field_x+radius && v.x < 0 ) ){
			v.x = -v.x * 0.99;
		}
		if ( ( r.y > field_y-radius && v.y >0 ) || ( r.y < -field_y+radius && v.y < 0 ) ){
			v.y = -v.y * 0.99;
		}
		if ( r.z < radius && v.z < 0 ){
			v.z = -v.z * 0.99;
			if ( r.z < radius ) r.z = radius;
		}
	}
};

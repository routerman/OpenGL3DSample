#include"F3.hpp"
#include<cmath>

F3::F3(){
	x=y=z=0;
}

void F3::set(float x,float y,float z){
	this->x=x;
	this->y=y;
	this->z=z;
}

//回転行列
F3 F3::rot(F3 d,F3 n, float arg){
	F3 s;
	float cost=cos(arg),sint=sin(arg);
	s.x=  ( n.x*n.x*(1-cost) + cost	     )*d.x
		+ ( n.x*n.y*(1-cost) - sint*n.z )*d.y
		+ ( n.x*n.z*(1-cost) + sint*n.y )*d.z;

	s.y=  ( n.y*n.x*(1-cost) + sint*n.z )*d.x
		+ ( n.y*n.y*(1-cost) + cost	     )*d.y
		+ ( n.y*n.z*(1-cost) - sint*n.x )*d.z;

	s.z=  ( n.z*n.x*(1-cost) - sint*n.y )*d.x
		+ ( n.z*n.y*(1-cost) + sint*n.x )*d.y
		+ ( n.z*n.z*(1-cost) + cost	     )*d.z;
	return s;
}



//外積
F3 F3::cross(F3 v1,F3 v2){
	F3 s;
	s.x=v1.y*v2.z-v1.z*v2.y;
	s.y=v1.z*v2.x-v1.x*v2.z;
	s.z=v1.x*v2.y-v1.y*v2.x;
	return s;
}
//ベクトルの大きさ
float F3::range(F3 a){
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}
//単位ベクトルにする
F3 F3::unit(F3 v){
	F3 s;
	float u;
	u=range(v);
	if(u==0){
		return v;
	}else{
		s.x=v.x/u;
		s.y=v.y/u;
		s.z=v.z/u;
		return s;
	}
}

// + operator
F3 F3::operator+(F3 a)
{
	a.x = x + a.x;
	a.y = y + a.y;
	a.z = z + a.z;
	return a;
}

// - operator
F3 F3::operator-(F3 a)
{
	a.x = x - a.x;
	a.y = y - a.y;
	a.z = z - a.z;
	return a;
}

// * operator(内積)
float F3::operator*(F3 a)
{
	return x*a.x + y*a.y + z*a.z;
}


// += operator
F3 F3::operator+=(F3 a)
{
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

// * operator scholor 
F3 operator*(float a,F3 b)
{
	b.x *= a;
	b.y *= a;
	b.z *= a;
	return b;
}



F3::F3(float x,float y,float z){
	 this->x=x;
	 this->y=y;
	 this->z=z;
 }
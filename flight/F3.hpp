#pragma once
class F3{
public:
	float x,y,z;	
	void set(float x,float y,float z);
	static F3 rot(F3 a,F3 b,float arg);
	static F3 cross(F3 v1,F3 v2);
	static float range(F3 a);
	static F3 unit(F3 v);
	//operator
	F3 operator+(F3 a);
	F3 operator-(F3 a);
	float operator*(F3 a);
	friend F3 operator*(float a,F3 b);
	F3 operator+=(F3 a);
	F3(float x,float y,float z);
	F3();
private:
};



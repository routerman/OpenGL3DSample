//#pragma once
#include "Include.hpp"
#include "common.hpp"
#include "Machine.hpp"
#include "Bullet.hpp"
#include "Bomb.hpp"

class Fighter:public Machine{
private:
	//int bullet;
	float f;
	float c;
	float pitch;
	float roll;
	//float yaw;
	int comand;
public:
	int bombwait;
	//model
	void move();
	void model();
	void shoot();

	void dropbomb(list<Bomb> *a);
	//view
	void drow();

	//control
	void control(int keystat);
	void drive(Fighter target);

	Fighter();
};
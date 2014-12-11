#pragma once
#include "Include.hpp"
#include "common.hpp"
#include "Machine.hpp"
#include "Bullet.hpp"
#include "Bomb.hpp"
class Bomb;

class Fighter : public Machine{
private:
	string name;
	int hp;
	float f;
	float max_f;
	float c;
	float pitch;
	float roll;
	//float yaw;
	int command;
	int bulletwait;
	int bombwait;
	int targetwait;
public:
	//model
	void init();
	void move();
	void model();
	void shoot();

	void shoot(list<Bullet> *bullets);
	void dropbomb(list<Bomb> *bombs);
	//view
	void drow();

	//control
	void control(int keystat);
	Fighter* getTarget( list<Fighter> *enemys );
	void drive(list<Fighter> *enemys);
	//int bullet;
	void decreaseHP(unsigned int damage);

	Fighter();
};

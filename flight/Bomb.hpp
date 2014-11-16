#pragma once
#include "Include.hpp"
#include "common.hpp"
#include "Weapon.hpp"
#include "F3.hpp"
#include "Fighter.hpp"

class Fighter;

class Bomb:public Weapon{
private:
	WeaonState state;
	int time;
	double radius;
public:
	WeaonState getState();
	void move();
	void model( list<Fighter> *enemys );
	void drow();
	void fire(F3 r,F3 d,F3 v);
	Bomb();
	~Bomb();
};

#pragma once
#include "Include.hpp"
#include "common.hpp"
#include "Weapon.hpp"
#include "F3.hpp"

class Bomb:public Weapon{
private:
	
public:
	int time;
	void move();
	void model();
	void drow();
	void fire(F3 r,F3 d,F3 v);
	Bomb();
	~Bomb();
};

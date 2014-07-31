#pragma once
#include "Include.hpp"
#include "F3.hpp"
#include "Object.hpp"
enum WeaonState{NOUSE,MOVE,FIRE};

class Weapon : public Object{
public:
	bool active;
	WeaonState state;
	int time;
	int whose;//who did fire?

	void model();
	void move();
	void drow();
	void fire();
	Weapon();
};
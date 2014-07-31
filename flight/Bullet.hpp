#pragma once
#include "Include.hpp"
#include"Weapon.hpp"
#include"F3.hpp"

class Bullet:public Weapon{
private:
	
public:
	void move();
	void drow();
	void fire(F3 r,F3 d,F3 v);
	Bullet();
};
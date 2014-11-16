//#pragma once
#include "Include.hpp"
#include "common.hpp"
#include "F3.hpp"
#include "Fighter.hpp"
#include "Bullet.hpp"
#include "Bomb.hpp"
#include "Surface.hpp"

#define ENEMY 1


//"0"の時はキーイベントものってことにするか。
class Game{
private:
	F3 eye,centor,up;
	int type;//アクションもの、キーイベントもの
	unsigned int keystat;
	int viewpoint;
	int time_interval;
	Fighter fighter;
	list<Fighter> enemylist;
	list<Bullet> bulletlist;
	list<Bomb> bomblist;
	list<Surface> surfacelist;
public:
	Fighter getFighter(){return fighter;};
	Game(int gametype);
	int time;
	void stage();
	void display();
	void timer();
	void init();
	void keydown(int key);
	void keyup(int key);
	void control();	
};

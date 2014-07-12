#pragma once
#include "Include.hpp"
#include "Player.hpp"

class Team{
private:
	int player_count;
	double gx,gy;
public:
	char name[32];
	int id;
	int score;
	//Player player[11];
	int getId(){
		return this->id;
	}
	Team(int id,char* name){
		this->id = id;
		char *str;
		str = this->name;
		while( *str++ = *name++);
		score=0;
		//player_count=0;
	}
	~Team(){
	}
	void addPlayer(Player *player){
		//this->player[player_count]=player;
		//player_count++;
	}
	void plan(Ball *ball, Team *team){
		for (int i = 0; i < 2*max_player; i++){
			if( player[i]->getTeamId() == this->id ) player[i]->setMode(BACK);
		}
		int player_id[2]={0};
		double tmp;
		double min_distance[2];
		min_distance[0] = 100000;
		min_distance[1] = 1000000;
		for (int i = 0; i < 2*max_player; i++){
			if( player[i]->getTeamId() != this->id ) continue;
			tmp = player[i]->getDistance(ball);
			if( min_distance[1] > tmp){
				if( min_distance[0] > tmp ){
					min_distance[0] = tmp;
					player_id[0] = i;
					continue;
				}
				min_distance[1] = tmp;
				player_id[1] = i;
			}
		}
		player[player_id[0]]->setMode(CHASE);
		player[player_id[1]]->setMode(CHASE);
		//we can control a player if assign 1 to i
		//for (int i = 0; i < 2*max_player; i++){
		for (int i = 1; i < 2*max_player; i++){
			if( player[i]->getTeamId() != this->id ) continue;
			player[i]->control(ball);
		}
	}
};

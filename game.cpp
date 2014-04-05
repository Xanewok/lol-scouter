#include <cstdio>
#include "cJSON/cJSON.h"
#include "game.hpp"

/* 	Mirrors the Riot API game structure */
game::game(char *buf, int size)
{
	
}

game::game(int game_id, int summoner_id)
{
	this->game_id = game_id;
	this->summoner_id = summoner_id;
}

bool game::operator<(const game &g) const
{
	return game_id + summoner_id < g.game_id + g.summoner_id;
}

player::player(int champion_id, int team_id, int summoner_id) : champion_id(champion_id), team_id(team_id), summoner_id(summoner_id) {}
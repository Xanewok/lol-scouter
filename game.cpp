#include <cstdio>
#include "cJSON/cJSON.h"
#include "game.hpp"

/* 	Mirrors the Riot API game structure with the exception of moving the individual
	player to fellow_players group (making the group having 10 members) */
game::game(char *buf, int size)
{
		
}

bool game::operator<(const game &g) const
{
	return game_id < g.game_id;
}

player::player(int champion_id, int team_id, int summoner_id) : champion_id(champion_id), team_id(team_id), summoner_id(summoner_id)
{
}

/*
struct player {
	int champion_id, team_id, summoner_id;
	player(int champion_id, team_id, summoner_id) : champion_id(champion_id), team_id(team_id), summoner_id(summoner_id) {}
};
*/
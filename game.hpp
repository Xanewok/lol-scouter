#include <cstdio>
#include "cJSON/cJSON.h"

#ifndef GAME_HPP
#define GAME_HPP

/* 	Mirrors the Riot API game structure with the exception of moving the individual
	player to fellow_players group (making the group having 10 members) */
struct game {
	char *internal_string;
	int game_id;

	game(char *buf, int size);
	bool operator<(const game &g) const;
};

struct player {
	int champion_id, team_id, summoner_id;

	player(int champion_id, int team_id, int summoner_id);
};

struct raw_stats {
	
};
#endif
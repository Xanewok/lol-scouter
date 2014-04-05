#include <cstdio>
#include "cJSON/cJSON.h"

#ifndef GAME_HPP
#define GAME_HPP

/* 	Mirrors the Riot API game structure */
struct game {
	char *internal_string;
	long long int game_id, summoner_id;

	game(char *buf, int size);
	game(int game_id, int summoner_id);
	bool operator<(const game &g) const;
};

struct player {
	int champion_id, team_id, summoner_id;

	player(int champion_id, int team_id, int summoner_id);
};

struct raw_stats {
	
};
#endif
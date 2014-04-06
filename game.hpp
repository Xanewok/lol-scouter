#include <cstdio>
#include "cJSON/cJSON.h"

#ifndef GAME_HPP
#define GAME_HPP

struct player {
	int champion_id, team_id, summoner_id;

	player(int champion_id, int team_id, int summoner_id);
};

struct raw_stats {
	int champions_killed, num_deaths, assists, minions_killed, neutral_minions_killed, gold_earned, level, time_played;
	bool win;
};

/* 	Mirrors the Riot API game structure */
struct game {
	long long int game_id, summoner_id, create_date;
	int champion_id, spell1, spell2, team_id, map_id, fellow_players_count;
	char sub_type[20];
	raw_stats *stats;
	player *fellow_players[10];

	game(cJSON *root, int summoner_id, bool &desired);
	~game();
	bool operator<(const game &g) const;
};

#endif
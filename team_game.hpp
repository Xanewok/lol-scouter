#include "cJSON/cJSON.h"

#ifndef TEAM_GAME_HPP
#define TEAM_GAME_HPP

struct player_perspective {
	long long int summoner_id;
	int champion_id, team_id, spell1, spell2, champions_killed, num_deaths, assists, minions_killed, neutral_minions_killed, gold_earned, level, time_played;
	bool win;
	
	player_perspective(int summoner_id, int champion_id, int team_id);
	player_perspective();
	player_perspective(cJSON *root, int summoner_id);
};

/* 	Modified Riot API game structure */
struct team_game {
	long long int game_id, create_date;
	int map_id, player_count, found_player_count;
	char sub_type[20];
	player_perspective *player_perspectives[10];

	team_game(cJSON *root, int summoner_id);
	team_game(int game_id);
	~team_game();
	void print_short_description();
	bool operator<(const team_game &g) const;
};

struct team_game_comparator {
	bool operator() (team_game* const& l, team_game* const& r);
};

#endif
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "cJSON/cJSON.h"
#include "game.hpp"

/* Attemps to create a game object parsing the JSON string */
game::game(cJSON *root, int summ_id, bool &desired)
{
	desired = false;
	summoner_id = summ_id;
	stats = new raw_stats;

	game_id = cJSON_GetObjectItem(root, "gameId")->valuedouble;
	create_date = cJSON_GetObjectItem(root, "createDate")->valuedouble;
	champion_id = cJSON_GetObjectItem(root, "championId")->valueint;
	spell1 = cJSON_GetObjectItem(root, "spell1")->valueint;
	spell2 = cJSON_GetObjectItem(root, "spell2")->valueint;
	team_id = cJSON_GetObjectItem(root, "teamId")->valueint;

	strncpy(sub_type, cJSON_GetObjectItem(root, "subType")->valuestring, 20);
	map_id = cJSON_GetObjectItem(root, "mapId")->valueint;
	if 	(map_id == 1 && (!strcmp(sub_type, "NONE") || !strcmp(sub_type, "NORMAL") ||
		!strcmp(sub_type, "RANKED_SOLO_5x5") || !strcmp(sub_type, "RANKED_TEAM_5x5")))
		desired = true;


	cJSON *fellow_players_json = cJSON_GetObjectItem(root, "fellowPlayers");
	if (fellow_players_json != 0) { // players can be playing alone
		fellow_players_count = cJSON_GetArraySize(fellow_players_json);
		for (int j = 0; j < cJSON_GetArraySize(fellow_players_json); ++j) {
			cJSON *player_json = cJSON_GetArrayItem(fellow_players_json, j); // player_json is a whole player entry

			int champ_id, summ_id, team_id;
			champ_id = cJSON_GetObjectItem(player_json, "championId")->valueint;
			summ_id = cJSON_GetObjectItem(player_json, "summonerId")->valuedouble;
			team_id = cJSON_GetObjectItem(player_json, "teamId")->valueint;
			fellow_players[j] = new player(champ_id, team_id, summ_id);
		}
	}

	/* int champions_killed, num_deaths, assists, gold, level, time_played, win; */
	cJSON *raw_statistics = cJSON_GetObjectItem(root, "stats");

	cJSON *handle;
	/* The data might not be present in the JSON string, thus we want to check if it's there first */
	handle = cJSON_GetObjectItem(raw_statistics, "championsKilled");// K
	stats->champions_killed = handle != 0? handle->valueint : 0;

	handle = cJSON_GetObjectItem(raw_statistics, "numDeaths");		// D
	stats->num_deaths = handle != 0? handle->valueint : 0;

	handle = cJSON_GetObjectItem(raw_statistics, "assists");		// A
	stats->assists = handle != 0? handle->valueint : 0;

	handle = cJSON_GetObjectItem(raw_statistics, "minionsKilled");
	stats->minions_killed = handle != 0? handle->valueint : 0;

	handle = cJSON_GetObjectItem(raw_statistics, "neutralMinionsKilled");
	stats->neutral_minions_killed = handle != 0? handle->valueint : 0;

	stats->gold_earned = cJSON_GetObjectItem(raw_statistics, "goldEarned")->valueint;
	stats->level = cJSON_GetObjectItem(raw_statistics, "level")->valueint;
	stats->time_played = cJSON_GetObjectItem(raw_statistics, "timePlayed")->valueint;
	stats->win = cJSON_GetObjectItem(raw_statistics, "win")->type; // from cJSON.h: #define cJSON_False 0 #define cJSON_True 1
}

game::~game()
{
	for (int i = 0; i < 10; ++i)
		if (fellow_players[i])
			free(fellow_players[i]);
}

bool game::operator<(const game &g) const
{
	return game_id + summoner_id < g.game_id + g.summoner_id;
}

player::player(int champion_id, int team_id, int summoner_id) : champion_id(champion_id), team_id(team_id), summoner_id(summoner_id) {}

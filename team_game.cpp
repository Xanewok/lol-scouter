#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "cJSON/cJSON.h"
#include "team_game.hpp"
#include "static_data.h"

player_perspective::player_perspective(int summoner_id, int champion_id, int team_id) :  summoner_id(summoner_id), champion_id(champion_id), team_id(team_id)
{
	spell1 = -1; // temporary indicator that it's an incomplete type
}
player_perspective::player_perspective()
{
	spell1 = -1; // temporary indicator that it's an incomplete type
}

player_perspective::player_perspective(cJSON *root, int summ_id)
{
	summoner_id = summ_id;
	champion_id = cJSON_GetObjectItem(root, "championId")->valueint;
	team_id = cJSON_GetObjectItem(root, "teamId")->valueint;
	spell1 = cJSON_GetObjectItem(root, "spell1")->valueint;
	spell2 = cJSON_GetObjectItem(root, "spell2")->valueint;
	/* int champions_killed, num_deaths, assists, gold, level, time_played, win; */
	cJSON *stats_json = cJSON_GetObjectItem(root, "stats");

	cJSON *handle;
	/* The data might not be present in the JSON string, thus we want to check if it's there first */
	handle = cJSON_GetObjectItem(stats_json, "championsKilled");// K
	champions_killed = handle != 0? handle->valueint : 0;

	handle = cJSON_GetObjectItem(stats_json, "numDeaths");		// D
	num_deaths = handle != 0? handle->valueint : 0;

	handle = cJSON_GetObjectItem(stats_json, "assists");		// A
	assists = handle != 0? handle->valueint : 0;

	handle = cJSON_GetObjectItem(stats_json, "minionsKilled");
	minions_killed = handle != 0? handle->valueint : 0;

	handle = cJSON_GetObjectItem(stats_json, "neutralMinionsKilled");
	neutral_minions_killed = handle != 0? handle->valueint : 0;

	gold_earned = cJSON_GetObjectItem(stats_json, "goldEarned")->valueint;
	level = cJSON_GetObjectItem(stats_json, "level")->valueint;
	time_played = cJSON_GetObjectItem(stats_json, "timePlayed")->valueint;
	win = cJSON_GetObjectItem(stats_json, "win")->type; // from cJSON.h: #define cJSON_False 0 #define cJSON_True 1
}

team_game::team_game(cJSON *root, int summ_id)
{
	game_id = cJSON_GetObjectItem(root, "gameId")->valuedouble;
	create_date = cJSON_GetObjectItem(root, "createDate")->valuedouble;
	strncpy(sub_type, cJSON_GetObjectItem(root, "subType")->valuestring, 20);
	map_id = cJSON_GetObjectItem(root, "mapId")->valueint;

	cJSON *fellow_players_json = cJSON_GetObjectItem(root, "fellowPlayers");
	if (fellow_players_json != 0)
		player_count = cJSON_GetArraySize(fellow_players_json) + 1; // players can be playing alone
	else
		player_count = 1;

	found_player_count = 1;
	player_perspectives[0] = new player_perspective(root, summ_id);

	for (int j = 1; j < player_count; ++j) {
		cJSON *player_json = cJSON_GetArrayItem(fellow_players_json, j-1); // player_json is a whole player entry

		int champ_id, summ_id, team_id;
		champ_id = cJSON_GetObjectItem(player_json, "championId")->valueint;
		summ_id = cJSON_GetObjectItem(player_json, "summonerId")->valuedouble;
		team_id = cJSON_GetObjectItem(player_json, "teamId")->valueint;
		player_perspectives[j] = new player_perspective(summ_id, champ_id, team_id);
	}
	// Not strictly needed
	for (int j = player_count; j < 10; ++j)
		player_perspectives[j] = 0;
}

team_game::~team_game()
{
	for (int i = 0; i < 10; ++i)
		if (player_perspectives[i] != 0)
			delete player_perspectives[i];
}

team_game::team_game(int game_id) : game_id(game_id) {}

void team_game::print_short_description()
{
	printf("game_id: %I64d\nplayer_count: %d, time_played: ~%d minutes\n", game_id, player_count, player_perspectives[0]->time_played / 60);
	//player_perspective *players[10] = player_perspectives;
	//printf("BLUE TEAM: %s\n", player_perspectives[0]->team_id == 100 && player_perspectives[0]->win? "(Won)" : "");
	
	int count = 0;
	for (int i = 0; i < player_count; ++i) {
		if (i == 0)
			printf("Blue Team: %s\n", (player_perspectives[0]->team_id == 100 && player_perspectives[0]->win) || (player_perspectives[0]->team_id == 200 && !player_perspectives[0]->win)? "(Won)" : "");
		else if (i == 5)
			printf("Purple Team: %s\n", (player_perspectives[0]->team_id == 200 && player_perspectives[0]->win) || (player_perspectives[0]->team_id == 100 && !player_perspectives[0]->win)? "(Won)" : "");
		for (int j = 0; j < player_count; ++j) {
			player_perspective *player = player_perspectives[j];

			if ((i < 5 && player->team_id == 100) ||
				(i >= 5 && player->team_id == 200))
				count++;
			//printf("i %d cnt %d", i, count);
			if (count == i + 1) {
				if (player->spell1 != -1) {
					
					printf("%s played %s (%s %s), KDA: %d/%d/%d CS: %d+%d\n",
							SUMMONER_NAMES[find_team(player->summoner_id)][find_summoner(player->summoner_id)], get_champion_name(player->champion_id),SUMMONER_SPELLS[player->spell1], SUMMONER_SPELLS[player->spell2],
							player->champions_killed, player->num_deaths, player->assists, player->minions_killed, player->neutral_minions_killed);
				} else
					printf("Summoner %I64d played %s\n", player->summoner_id, get_champion_name(player->champion_id));

				count = i >= 4? 5 : 0; // not 100% sure about 5 being here
				break;
			}
		}
	}
	/*
	for (int i = 0; i < player_count; i++) {
		player_perspective *player = player_perspectives[i];
		if (player->spell1 != -1) {
			printf("Summoner %I64d played %d on team %d for roughly %d minutes and %s, KDA: %d/%d/%d CS: %d+%d (Summoner spells: %s %s)\n",
					player->summoner_id, player->champion_id, player->team_id, player->time_played/60, player->win? "won" : "lost",
					player->champions_killed, player->num_deaths, player->assists, player->minions_killed, player->neutral_minions_killed, SUMMONER_SPELLS[player->spell1], SUMMONER_SPELLS[player->spell2]);
		} else {
			printf("Summoner %I64d played %d on team %d\n", player->summoner_id, player->champion_id, player->team_id);
		}
	}
	*/
}

bool team_game::operator<(const team_game &g) const
{
	return game_id < g.game_id;
}

bool team_game_comparator::operator() (team_game* const& l, team_game* const& r)
{
	return (*l) < (*r);
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
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

bool player_perspective::complete_info()
{
	return spell1 != -1;
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
					if (find_team(player->summoner_id) != -1)
							printf("%s played %s\n", SUMMONER_NAMES[find_team(player->summoner_id)][find_summoner(player->summoner_id)], get_champion_name(player->champion_id));
					else
						printf("Summoner %I64d played %s\n", player->summoner_id, get_champion_name(player->champion_id));

				count = i >= 4? 5 : 0; // not 100% sure about 5 being here
				break;
			}
		}
	}
}

void team_game::print_formatted_desc()
{
	//printf("s");
	player_perspective *first = player_perspectives[0];
	char seconds[3] = {'0'};
	if (first->time_played % 60 < 10)
		seconds[1] = (first->time_played % 60) + '0';
	else
		sprintf(seconds, "%d", first->time_played % 60);
	time_t game_date = create_date/1000;
	tm *data_str = localtime(&game_date);
	printf("%s%s\n(%s) %d:%s\n", asctime(data_str), TEAM_NAMES[find_team(first->summoner_id)], first->win? "Victory" : "Loss", first->time_played / 60, seconds);
	delete data_str;
	
	int cnt = 0;
	printf("%s side\n", first->team_id == 100? "Blue" : "Purple");
	for (int i = 0; i < player_count; ++i) {
		if (i == 5) putchar('\n');

		for (int j = 0; j < player_count; ++j) {
			player_perspective *jth_player = player_perspectives[j];
			
			if ((i < 5 && jth_player->team_id == first->team_id) || (i >= 5 && jth_player->team_id != first->team_id))
				cnt++;
			
			if (cnt == i + 1) {
				int nick_len = strlen(SUMMONER_NAMES[find_team(jth_player->summoner_id)][find_summoner(jth_player->summoner_id)]);
				char *indent;
				if (nick_len < 8)
					indent = (char *)"\t\t\t";
				else if (nick_len < 15)
					indent = (char *)"\t\t";
				else
					indent = (char *)"\t";
				char *tab = (char *)"\t", *dbl_tab = (char *)"\t\t";
				if (jth_player->complete_info()) {
					printf("%s%s%s%s[%c %c] %d/%d/%d\t%d+%d\n",
							SUMMONER_NAMES[find_team(jth_player->summoner_id)][find_summoner(jth_player->summoner_id)],
							indent,
							get_champion_name(jth_player->champion_id), 
							strlen(get_champion_name(jth_player->champion_id)) < 8? dbl_tab : tab,SUMMONER_SPELLS[jth_player->spell1][0], SUMMONER_SPELLS[jth_player->spell2][0],
							jth_player->champions_killed, jth_player->num_deaths, jth_player->assists, jth_player->minions_killed, jth_player->neutral_minions_killed);
				} else {
					if (find_team(jth_player->summoner_id) != -1) {
						printf("%s%s%s\n", SUMMONER_NAMES[find_team(jth_player->summoner_id)][find_summoner(jth_player->summoner_id)],
						indent, get_champion_name(jth_player->champion_id));
					} else
						printf("Summ. %I64d\t%s\n", jth_player->summoner_id, get_champion_name(jth_player->champion_id));
				}
				cnt = (i >= 4)? 5 : 0;

				break;
			}
		}
	}
}

bool team_game::operator<(const team_game &g) const
{
	return game_id < g.game_id;
}

bool team_game_comparator::operator() (team_game* const& l, team_game* const& r)
{
	return (*l) < (*r);
}

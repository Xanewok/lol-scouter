#include "game.hpp"
#include "constants.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <set>
#include "team_game.hpp"
#include "cJSON/cJSON.h"

#define MIN_TEAMMATES_COUNT 4

std::set<game *, game_comparator> game_history, player_ranked_game_history[NUM_TEAMS][NUM_TEAM_MEMBERS], player_normal_game_history[NUM_TEAMS][NUM_TEAM_MEMBERS];
std::set<team_game *, team_game_comparator> team_game_history;

int find_team(int summoner_id)
{
	for (int i = 0; i < NUM_TEAMS; i++)
		for (int j = 0; j < NUM_TEAM_MEMBERS; j++)
			if (TEAMS[i][j] == summoner_id)
				return i;
	return -1;
}

int find_summoner(int summoner_id)
{
	for (int i = 0; i < NUM_TEAMS; i++)
		for (int j = 0; j < NUM_TEAM_MEMBERS; j++)
			if (TEAMS[i][j] == summoner_id)
				return j;
	return -1;
}

void read_games_from_file(const char *name)
{
	FILE *fp;
	char *buf = 0;
	fp = fopen(name, "r");
	if (fp != 0) {
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		buf = (char *)malloc(size);

		if (buf == 0)
			printf("mem error\n");

		fread(buf, 1, size, fp);

		fclose(fp);
	} else
		return; // don't do stuff when we can't access the input data

	if (buf != 0) {
		cJSON *json = cJSON_Parse(buf);
		if (!json) {
			printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		} else {
			for (int i = 0; i < cJSON_GetArraySize(json); ++i) {
				cJSON *game_json = cJSON_GetArrayItem(json, i); // game_json is a whole game entry

				int summ_id = cJSON_GetObjectItem(game_json, "summonerId")->valuedouble;
				game *gam = new game(game_json, summ_id);

				game_history.insert(gam);
				//gam->print_short_description();
			}
		}
		cJSON_Delete(json);
		free(buf);
	}
}

void read_team_games_from_file(const char *name)
{
	FILE *fp;
	char *buf = 0;
	fp = fopen(name, "r");
	if (fp != 0) {
		fseek(fp, 0, SEEK_END);
		long size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		buf = (char *)malloc(size);

		if (buf == 0)
			printf("mem error\n");

		fread(buf, 1, size, fp);

		fclose(fp);
	} else
		return; // don't do stuff when we can't access the input data

	if (buf != 0) {
		cJSON *json = cJSON_Parse(buf);
		if (!json) {
			printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		} else {
			for (int i = 0; i < cJSON_GetArraySize(json); ++i) {
				cJSON *game_json = cJSON_GetArrayItem(json, i); // game_json is a whole game entry

				int summ_id = cJSON_GetObjectItem(game_json, "summonerId")->valuedouble;
				int game_id = cJSON_GetObjectItem(game_json, "gameId")->valuedouble;
				
	
				std::set<team_game *, team_game_comparator>::iterator it = team_game_history.find(new team_game(game_id));
				if (it == team_game_history.end()) // Nie znaleziono
					team_game_history.insert(new team_game(game_json, summ_id));
				else {
					for (int i = 1; i < (*it)->player_count; ++i) {
						if ((*it)->player_perspectives[i]->summoner_id == summ_id) {
							delete (*it)->player_perspectives[i];
							(*it)->found_player_count++;
							(*it)->player_perspectives[i] = new player_perspective(game_json, summ_id);
							break;
						}
					}
				}
			}
		}
		cJSON_Delete(json);
		free(buf);
	}
}

bool read_game_history()
{
	int idx;
	char input_file[20];
	for (idx = 0; ; ++idx) {
		sprintf(input_file, "output_%d.txt", idx);
		FILE *fp = fopen(input_file, "r");
		if (fp != 0)
			continue;
		else
			break;
	}

	if (idx == 0)
		return false;
	else {
		sprintf(input_file, "output_%d.txt", idx-1);
		read_games_from_file(input_file);
		read_team_games_from_file(input_file);
	}

	return true;
}

int main()
{
	if (!read_game_history()) {
		printf("There is no input file (output_<num>.txt).\n");
		return -1; // Couldn't find the input, aborting
	}

	for (std::set<game *, game_comparator>::iterator it = game_history.begin(); it != game_history.end(); ++it) {
		int bgt_team = find_team((*it)->summoner_id);

		// Check if there's a (almost) full BGT team in the game
		if ((*it)->fellow_players_count == 9) { // only here because I screwed up while writing the scouter program, can't fix it yet
			if ((!strcmp((*it)->sub_type, "NORMAL"))) {
				player_normal_game_history[bgt_team][find_summoner((*it)->summoner_id)].insert(*it);
			} else if (!strcmp((*it)->sub_type, "NONE") || !strcmp((*it)->sub_type, "RANKED_TEAM_5x5") || !strcmp((*it)->sub_type, "RANKED_SOLO_5x5")) {
				player_ranked_game_history[bgt_team][find_summoner((*it)->summoner_id)].insert(*it);
			}
		}	
	}
	
	int bgt_practice_count = 0;
	for (std::set<team_game *, team_game_comparator>::iterator it = team_game_history.begin(); it != team_game_history.end(); ++it) {
		if ((*it)->found_player_count >= 4) {
			bgt_practice_count++;
			//(*it)->print_short_description(); - use that to see the overview of a game including personal stats for each person etc.
		}
	}

	printf("size of the initial (non-unique) game history: %d\n", game_history.size());
	printf("size of the team (unique) game history: %d\n", team_game_history.size());
	printf("number of BGT games with %d+ members: %d\n", MIN_TEAMMATES_COUNT, bgt_practice_count);
	printf("sizes of teammates normal history:\n");
	for (int i = 0; i < NUM_TEAMS; i++) {
		for (int j = 0; j < NUM_TEAM_MEMBERS; j++) {
			printf("%d\t", player_normal_game_history[i][j].size());
		}
		putchar('\n');
	}
	printf("sizes of teammates ranked history:\n");
	for (int i = 0; i < NUM_TEAMS; i++) {
		for (int j = 0; j < NUM_TEAM_MEMBERS; j++) {
			printf("%d\t", player_ranked_game_history[i][j].size());
		}
		putchar('\n');
	}

	return 0;
}

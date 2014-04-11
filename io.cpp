#include <cstdlib>
#include <cstring>
#include <set>
#include "cJSON/cJSON.h"
#include "game.hpp"
#include "team_game.hpp"
#include "io.hpp"

void read_games_from_file(std::set<game *, game_comparator> &game_history, const char *name)
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

void read_team_games_from_file(std::set<team_game *, team_game_comparator> &team_game_history, const char *name)
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

				//team_game *search_obj = new team_game(game_id);
				 // Memory leak here probably, not deleting the new team_game, altho it crashes when I try to work around it
				std::set<team_game *, team_game_comparator>::iterator it = team_game_history.find(new team_game(game_id));
				//delete search_obj;

				if (it == team_game_history.end()) // Nie znaleziono
					team_game_history.insert(new team_game(game_json, summ_id));
				else {
					/* player_perspective[0] is ALWAYS the first player we looked for who had the game in the history */
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

void save_to_file(std::set<game *, game_comparator > &game_history, const char *name)
{
	FILE *fp;
	fp = fopen(name, "w+");
	cJSON *root = cJSON_CreateArray();
	for (std::set<game *>::iterator it = game_history.begin(); it != game_history.end(); it++) {
		cJSON *game = cJSON_CreateObject();

		if ((*it)->fellow_players_count != 0) {
			cJSON *fellow_players = cJSON_CreateArray();

			for (int i = 0; i < (*it)->fellow_players_count; i++) {
				cJSON *player_entry = cJSON_CreateObject();
				cJSON_AddItemToObject(player_entry, "championId", cJSON_CreateNumber((*it)->fellow_players[i]->champion_id));
				cJSON_AddItemToObject(player_entry, "teamId", cJSON_CreateNumber((*it)->fellow_players[i]->team_id));
				cJSON_AddItemToObject(player_entry, "summonerId", cJSON_CreateNumber((*it)->fellow_players[i]->summoner_id));

				cJSON_AddItemToArray(fellow_players, player_entry);
			}

			cJSON_AddItemToObject(game, "fellowPlayers", fellow_players);
		}
		cJSON *stats = cJSON_CreateObject();
		cJSON_AddItemToObject(stats, "championsKilled", cJSON_CreateNumber((*it)->stats->champions_killed));
		cJSON_AddItemToObject(stats, "numDeaths", cJSON_CreateNumber((*it)->stats->num_deaths));
		cJSON_AddItemToObject(stats, "assists", cJSON_CreateNumber((*it)->stats->assists));
		cJSON_AddItemToObject(stats, "minionsKilled", cJSON_CreateNumber((*it)->stats->minions_killed));
		cJSON_AddItemToObject(stats, "neutralMinionsKilled", cJSON_CreateNumber((*it)->stats->neutral_minions_killed));
		cJSON_AddItemToObject(stats, "goldEarned", cJSON_CreateNumber((*it)->stats->gold_earned));
		cJSON_AddItemToObject(stats, "level", cJSON_CreateNumber((*it)->stats->level));
		cJSON_AddItemToObject(stats, "timePlayed", cJSON_CreateNumber((*it)->stats->time_played));
		cJSON_AddItemToObject(stats, "win", (*it)->stats->win? cJSON_CreateTrue() : cJSON_CreateFalse());
		cJSON_AddItemToObject(game, "stats", stats);
		// Custom object, used for data saving/loading convenience
		cJSON_AddItemToObject(game, "summonerId", cJSON_CreateNumber((*it)->summoner_id));

		cJSON_AddItemToObject(game, "gameId", cJSON_CreateNumber((*it)->game_id));
		cJSON_AddItemToObject(game, "createDate", cJSON_CreateNumber((*it)->create_date));
		cJSON_AddItemToObject(game, "championId", cJSON_CreateNumber((*it)->champion_id));
		cJSON_AddItemToObject(game, "spell1", cJSON_CreateNumber((*it)->spell1));
		cJSON_AddItemToObject(game, "spell2", cJSON_CreateNumber((*it)->spell2));
		cJSON_AddItemToObject(game, "teamId", cJSON_CreateNumber((*it)->team_id));
		cJSON_AddItemToObject(game, "mapId", cJSON_CreateNumber((*it)->map_id));
		cJSON_AddItemToObject(game, "subType", cJSON_CreateString((*it)->sub_type));

		cJSON_AddItemToArray(root, game);
	}
	char *out = cJSON_Print(root);
	fwrite(out, sizeof(char), strlen(out), fp);

	cJSON_Delete(root);
	free(out);
	fclose(fp);
}
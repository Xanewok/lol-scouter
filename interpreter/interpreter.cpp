#include "..\game.hpp"
#include "..\constants.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <set>

#define MIN_TEAMMATES_COUNT 4

std::set<game *, game_comparator> game_history, bgt_team_game_history, player_game_history[NUM_TEAMS][NUM_TEAM_MEMBERS];


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

void read_from_file(const char *name)
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
		read_from_file(input_file);
	}

	return true;
}

int main()
{
	if (!read_game_history()) {
		printf("Nie ma pliku z inputem, przerywam prace.\n");
		return -1; // Nie udalo sie odnalezc pliku z historia gier
	}

	for (std::set<game *, game_comparator>::iterator it = game_history.begin(); it != game_history.end(); ++it) {
		game gra = (**it);

		int bgt_team = find_team(gra.summoner_id);
		assert(bgt_team != -1);

		// Sprawdzamy czy jest to gra teamu BGT
		if (gra.fellow_players_count == 9) {
			if ((!strcmp(gra.sub_type, "NONE") || !strcmp(gra.sub_type, "RANKED_TEAM_5x5"))) {

				int count = 1;
				for (int i = 0; i < gra.fellow_players_count; i++) {
					player *gracz = gra.fellow_players[i];

					for (int j = 0; j < NUM_TEAM_MEMBERS; j++) {
						if (TEAMS[bgt_team][j] == (int)gracz->summoner_id) {
							count++;
							break;
						}
					}
				}
				//printf("count: %d (game_id: %d, summoner_id: %d, bgt_team: %d)\n", count, (int)gra.game_id, (int)gra.summoner_id, bgt_team);

				if (count >= MIN_TEAMMATES_COUNT) { // 4 lub 5
					// znalezlismy gre teamowa, warto ja przeanalizowac
					printf("Dodaje gre o game_id: %d\n", (int)gra.game_id);
					bgt_team_game_history.insert(*it);
				} else {
					player_game_history[bgt_team][find_summoner(gra.summoner_id)].insert(*it);
					// Znalezlismy gre teamowa, ale nikt z BGT nie gral, wiec zapiszmy to do solo ranked game

				}
			} else if (!strcmp(gra.sub_type, "NORMAL")) {
				// Znalezlismy normala
				player_game_history[bgt_team][find_summoner(gra.summoner_id)].insert(*it);

			} else if (!strcmp(gra.sub_type, "RANKED_SOLO_5x5")) {
				// Znalezlismy soloq
				player_game_history[bgt_team][find_summoner(gra.summoner_id)].insert(*it);
			}
		}
	}
	printf("size of the input game history: %d\n", game_history.size());
	printf("size of the bgt team game history: %d\n", bgt_team_game_history.size());
	printf("sizes of teammates history:\n");
	for (int i = 0; i < NUM_TEAMS; i++) {
		for (int j = 0; j < NUM_TEAM_MEMBERS; j++) {
			printf("%d\t", player_game_history[i][j].size());
		}
		putchar('\n');
	}

	return 0;
}

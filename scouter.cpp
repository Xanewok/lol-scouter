#include <cstdio>
#include <set>
#include <curl/curl.h>
#include <windows.h>
#include "cJSON/cJSON.h"
#include "game.hpp"
#include "static_data.h"
#include "network.h"
#include "io.hpp"

#define SINGLE_REQUEST_INTERVAL 1000
#define FULL_REQUEST_INTERVAL 10800000 // 3 hours * 60 minutes * 60 seconds * 1000 milliseconds

std::set<game *, game_comparator > game_history;



int main(int argc, char* argv[])
{
	printf("This product is not endorsed, certified or otherwise approved in any way by Riot Games, Inc. or any of its affiliates.\n\n"); // we need to put this somewhere else
	printf("Please input your Riot API key: ");
	char key[37];
	scanf("%s", key);

	CURL *curl;
	curl_global_init(CURL_GLOBAL_ALL);

	while (true) {
		curl = curl_easy_init();
		for (int i = 0; i < NUM_TEAMS; ++i) {
			for (int j = 0; j < NUM_TEAM_MEMBERS; ++j) {
				/* get_games reuses the same handle by resetting the options and executing an updated query */
				char *games = get_games(curl, TEAMS[i][j], key);
				cJSON *json = cJSON_Parse(games);
				free(games);

				if (!json) {
					printf("Error before: [%s]\n",cJSON_GetErrorPtr());
				} else {
					cJSON *games_json = cJSON_GetObjectItem(json, "games");

					for (int k = 0; k < cJSON_GetArraySize(games_json); ++k) {
						cJSON *game_json = cJSON_GetArrayItem(games_json, k);

						game *gam = new game(game_json, TEAMS[i][j]);
						/* We only need Summoner's Rift games with 10 players */
						if (gam->map_id == 1 && gam->fellow_players_count == 9 &&
								(!strcmp(gam->sub_type, "NONE") || !strcmp(gam->sub_type, "NORMAL") ||
								!strcmp(gam->sub_type, "RANKED_SOLO_5x5") || !strcmp(gam->sub_type, "RANKED_TEAM_5x5"))) {

							game_history.insert(gam);
							gam->print_short_description();
						}
					}
				}
				cJSON_Delete(json);

				Sleep(SINGLE_REQUEST_INTERVAL); // total requests: 80 (Rate Limit(s): 500 request(s) every 10 minute(s) / 10 request(s) every 10 second(s))
			}
		}
		curl_easy_cleanup(curl);
		printf("Size of the set: %d\n", game_history.size());

		char output_file[20];
		for (int i = 0; ; i++) {
			sprintf(output_file, "output_%d.txt", i);
			FILE *fp = fopen(output_file, "r+");
			if (fp == 0) { // we're looking for the next available file for creation
				fclose(fp);
				break;
			}
		}
		save_to_file(game_history, output_file);
		Sleep(FULL_REQUEST_INTERVAL); // interval between full fetch sessions
	}
	curl_global_cleanup();

	return 0;
}
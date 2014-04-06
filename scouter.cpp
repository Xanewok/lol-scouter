#include <cstdio>
#include <set>
#include <curl/curl.h>
#include <windows.h>
#include "cJSON/cJSON.h"
#include "game.hpp"

#define NUM_TEAMS 16
#define NUM_MEMBERS 5

std::set<game *> game_history;

/* Temporary team holder */
const int teams[NUM_TEAMS][NUM_MEMBERS] =	{{19381878, 19739336, 26154202, 29945492, 24423271},/* Random Five */
											{28139728, 19748815, 29943215, 31171162, 19315755},	/* Purple Siege Minions */
											{27458826, 24207536, 25163092, 26949381, 27101500},	/* Nawiedzeni */
											{34643485, 24753771, 29997052, 27983360, 25053140},	/* Kuba znowu w tyle */
											{21176576, 25227171, 35438455, 25904226, 34200780},	/* Kurczak Tajland */
											{20250492, 19370276, 19031155, 20153342, 19273528},	/* Slodkie Ziemniaczki Prime */
											{19446487, 19406317, 19708474, 19383763, 19393463},	/* Nasos pls */
											{30976455, 33093599, 20339886, 24793079, 24450367},	/* Bronzy na bocie */
											{22311549, 39633121, 22960348, 23760345, 23042154},	/* Prawilna Piatka */
											{19754347, 20976729, 346822, 33769371, 20557017},	/* Liniowo Niezalezni */
											{23940204, 39053694, 24330119, 25444452, 31181645},	/* Kenson Squad */
											{24368336, 21191718, 19878565, 22662299, 19863192},	/* Pan da RP-ki */
											{20095198, 20072326, 19928525, 19940196, 19811451},	/* AGT */
											{28190588, 23918752, 23982127, 27797169, 24094826}, /* Kule po dziurach */
											{33863101, 28545674, 33863166, 34333251, 30972927},	/* Team Erasers */
											{20539748, 20493466, 21803047, 26121772, 27824463}};/* Baited */

int find_team(int summoner_id)
{
	for (int i = 0; i < NUM_TEAMS; i++)
		for (int j = 0; j < NUM_MEMBERS; j++)
			if (teams[i][j] == summoner_id)
				return i;
	return -1;
}

struct MemoryStruct {
	char *memory;
	size_t size;
	MemoryStruct(char *ptr = (char *)malloc(1), int size = 0) : memory(ptr), size(size) {} // not sure about this tho
	~MemoryStruct()
	{
		if (memory != 0)
			free(memory);
	}
	void reset()
	{
		free(memory);
		memory = (char *)malloc(1);
		size = 0;
	}
};

/* http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTWRITEFUNCTION */
size_t write_func( char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t realsize = size * nmemb;

	struct MemoryStruct *mem = (struct MemoryStruct *)userdata;
	mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);

	if (mem->memory == 0) {
		/* out of memory */
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), ptr, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize; // return bytes taken care of by func
}

void init()
{

}

void save_to_file(const char *name)
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
	free(out);
	fclose(fp);
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
	}

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
				gam->print_short_description();
			}
		}
		free(buf);
	}
}

int main(int argc, char* argv[])
{
	printf("This product is not endorsed, certified or otherwise approved in any way by Riot Games, Inc. or any of its affiliates.\n\n"); // we need to put this somewhere else
	printf("Please input your Riot API key: ");
	char key[37];
	scanf("%s", key);

	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_ALL);

	char query[200];
	MemoryStruct chunk;

	curl = curl_easy_init();
	if (curl) {
		/* Tell libcurl to *not* verify the peer. With libcurl you disable this with */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		for (int i = 0; i < NUM_TEAMS; ++i) {
			for (int j = 0; j < NUM_MEMBERS; ++j) {
				sprintf(query, "https://prod.api.pvp.net/api/lol/eune/v1.3/game/by-summoner/%d/recent?api_key=%s", teams[i][j], key);
				curl_easy_setopt(curl, CURLOPT_URL, query);

				/* Perform the request, res will get the return code */
				res = curl_easy_perform(curl);
				/* Check for errors */
				if (res != CURLE_OK)
					fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
				else {
					cJSON *json = cJSON_Parse(chunk.memory);

					if (!json) {
						printf("Error before: [%s]\n",cJSON_GetErrorPtr());
					} else {
						cJSON *games_json = cJSON_GetObjectItem(json, "games");

						for (int k = 0; k < cJSON_GetArraySize(games_json); ++k) {
							cJSON *game_json = cJSON_GetArrayItem(games_json, k); // game_json is a whole game entry

							game *gam = new game(game_json, teams[i][j]);

							if (gam->map_id == 1 && (!strcmp(gam->sub_type, "NONE") || !strcmp(gam->sub_type, "NORMAL") ||
											!strcmp(gam->sub_type, "RANKED_SOLO_5x5") || !strcmp(gam->sub_type, "RANKED_TEAM_5x5"))) {
								game_history.insert(gam);

								gam->print_short_description();
							}
						}
					}
					cJSON_Delete(json);
					chunk.reset();
				}
				Sleep(1100); // total requests: 80 (Rate Limit(s): 500 request(s) every 10 minute(s) / 10 request(s) every 10 second(s))
			}
		}
		save_to_file("testing_output.txt");
		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return 0;
}
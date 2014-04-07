#include "..\game.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
//#include <windows.h>
#include <set>

#define MIN_TEAMMATES_COUNT 4
#define NUM_TEAMS 16
#define NUM_MEMBERS 5

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

std::set<game *, game_comparator> game_history, bgt_team_game_history, player_game_history[NUM_TEAMS][NUM_MEMBERS];


int find_team(int summoner_id)
{
	for (int i = 0; i < NUM_TEAMS; i++)
		for (int j = 0; j < NUM_MEMBERS; j++)
			if (teams[i][j] == summoner_id)
				return i;
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

		// Sprawdzamy czy jest to gra teamu BGT
		if (gra.fellow_players_count == 9) {
			if ((!strcmp(gra.sub_type, "NONE") || !strcmp(gra.sub_type, "RANKED_TEAM_5x5"))) {
				int bgt_team = find_team(gra.summoner_id);
				assert(bgt_team != -1);

				int count = 1;

				for (int i = 0; i < gra.fellow_players_count; i++) {
					player *gracz = gra.fellow_players[i];
	
					for (int j = 0; j < NUM_MEMBERS; j++) {
						if (teams[bgt_team][j] == (int)gracz->summoner_id) {
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
					player_game_history[bgt_team]
					// Znalezlismy gre teamowa, ale nikt z BGT nie gral, wiec zapiszmy to do solo ranked game
					
				}
			} else if (!strcmp(gra.sub_type, "NORMAL")) {
				// Znalezlismy normala
				
			} else if (!strcmp(gra.sub_type, "RANKED_SOLO_5x5")) {
				soloq:
				// Znalezlismy soloq
			}
		}
	}
	printf("size of the input game history: %d\n", game_history.size());
	printf("size of the bgt team game history: %d\n", bgt_team_game_history.size());

	return 0;
}

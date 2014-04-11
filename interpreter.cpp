#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <set>
#include "cJSON/cJSON.h"
#include "io.hpp"
#include "static_data.h"
#include "game.hpp"
#include "team_game.hpp"

#define MIN_TEAMMATES_COUNT 4

std::set<game *, game_comparator> game_history, player_ranked_game_history[NUM_TEAMS][NUM_TEAM_MEMBERS], player_normal_game_history[NUM_TEAMS][NUM_TEAM_MEMBERS];
std::set<team_game *, team_game_comparator> team_game_history;

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
		read_games_from_file(game_history, input_file);
		read_team_games_from_file(team_game_history, input_file);
	}

	return true;
}

void process_games() {
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
}

void process_teamgames(int &count) {
	count = 0;
	for (std::set<team_game *, team_game_comparator>::iterator it = team_game_history.begin(); it != team_game_history.end(); ++it) {
		if ((*it)->found_player_count >= 4) {
			count++;
		}
	}
}

int main()
{
printf("test\n");
	if (!read_game_history()) {
		printf("There is no input file (output_<num>.txt).\n");
		return -1; // Couldn't find the input, aborting
	}
printf("test\n");
	int bgt_count;
	process_games();
	process_teamgames(bgt_count);


	printf("size of the initial (non-unique) game history: %d\n", game_history.size());
	printf("size of the team (unique) game history: %d\n", team_game_history.size());
	printf("number of BGT games with %d+ members: %d\n", MIN_TEAMMATES_COUNT, bgt_count);
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

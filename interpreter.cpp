#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <set>
#include <vector>
#include <algorithm>
#include "cJSON/cJSON.h"
#include "io.hpp"
#include "static_data.h"
#include "game.hpp"
#include "team_game.hpp"

#define MIN_TEAMMATES_COUNT 4

std::set<game *, game_comparator> game_history;
std::set<team_game *, team_game_comparator> team_game_history, bgt_team_games[16];

struct champ_stats {
	int champion_id, w, l;
	float k, d, a, cs;
	champ_stats(int champion_id, int k, int d, int a, int cs, bool w) : champion_id(champion_id), k(k), d(d), a(a), cs(cs)
	{
		w = 0;
		l = 0;
		if (w)
			w++;
		else
			++l;
	}
	champ_stats(game *gam)
	{
		w = 0;
		l = 0;
		if (gam->stats->win)
			w++;
		else
			++l;
		champion_id = gam->champion_id;
		k = gam->stats->champions_killed;
		d = gam->stats->num_deaths;
		a = gam->stats->assists;
		cs = gam->stats->minions_killed + gam->stats->neutral_minions_killed;
	}
	float winrate() const
	{
		return (float(w) * 100) / (float(w) + float(l));
	}
	void update_stats(int ka, int da, int aa, int csa, bool wa)
	{
		k = ((k * float(w + l)) + ka) / (w + l + 1);
		d = ((d * float(w + l)) + da) / (w + l + 1);
		a = ((a * float(w + l)) + aa) / (w + l + 1);
		cs = ((cs * float(w + l)) + csa) / (w + l + 1);
		
		if (w)
			++w;
		else
			++l;
	}
	void update_stats (game *gam)
	{
		int uk = gam->stats->champions_killed;
		int ud = gam->stats->num_deaths;
		int ua = gam->stats->assists;
		int ucs = gam->stats->minions_killed + gam->stats->neutral_minions_killed;
		
		k = ((k * float(w + l)) + uk) / (w + l + 1);
		d = ((d * float(w + l)) + ud) / (w + l + 1);
		a = ((a * float(w + l)) + ua) / (w + l + 1);
		cs = ((cs * float(w + l)) + ucs) / (w + l + 1);
		
		if (gam->stats->win)
			w++;
		else
			++l;
	}
	
	bool operator<(const champ_stats &r) const
	{
		if (w + l != r.w + r.l)
			return w + l < r.w + r.l;
		else if (winrate() != r.winrate())
			return winrate() < r.winrate();
		else
			return champion_id < r.champion_id;
	}
	
	bool operator==(const champ_stats &r) const
	{
		return champion_id == r.champion_id;
	}
	
	champ_stats & operator=(const champ_stats &r)
	{
		if (this == &r)
			return *this;
		champion_id = r.champion_id;
		w = r.w;
		l = r.l;
		k = r.k;
		d = r.d;
		a = r.a;
		cs = r.cs;
		return *this;
	}
};

std::vector<champ_stats> player_ranked_stats[NUM_TEAMS][NUM_TEAM_MEMBERS], player_normal_stats[NUM_TEAMS][NUM_TEAM_MEMBERS];

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

bool read_full_game_history()
{
	int idx;
	char input_file[20];
	
	idx = 0;
	for (; ; ++idx) {
		sprintf(input_file, "output_%d.txt", idx);
		FILE *fp = fopen(input_file, "r");
		if (fp == 0)
			break;
		else {
			read_games_from_file(game_history, input_file);
			read_team_games_from_file(team_game_history, input_file);
		}
	}

	if (idx == 0)
		return false;

	return true;
}

void process_games() {
	for (std::set<game *, game_comparator>::iterator it = game_history.begin(); it != game_history.end(); ++it) {
		int team = find_team((*it)->summoner_id);
		int summ = find_summoner((*it)->summoner_id);

		// Check if there's a (almost) full BGT team in the game
		if ((*it)->fellow_players_count == 9) { // only here because I screwed up while writing the scouter program, can't fix it yet
			champ_stats obj = champ_stats((*it));
			if ((!strcmp((*it)->sub_type, "NORMAL"))) {
			
				std::vector<champ_stats>::iterator vec_it = std::find(player_normal_stats[team][summ].begin(), player_normal_stats[team][summ].end(), obj);
			
				if (vec_it == player_normal_stats[team][summ].end()) 
					player_normal_stats[team][summ].push_back(obj);
				else {
					//printf("normal\n");
					vec_it->update_stats(*it);
				}
					
					
			} else if (!strcmp((*it)->sub_type, "NONE") || !strcmp((*it)->sub_type, "RANKED_TEAM_5x5") || !strcmp((*it)->sub_type, "RANKED_SOLO_5x5")) {
				std::vector<champ_stats>::iterator vec_it = std::find(player_ranked_stats[team][summ].begin(), player_ranked_stats[team][summ].end(), obj);
				
				if (vec_it == player_ranked_stats[team][summ].end()) 
					player_ranked_stats[team][summ].push_back(obj);
				else {
					//printf("ranked\n");
					vec_it->update_stats(*it);
				}
			}
		}
	}
}

void process_teamgames(int &game_cnt) {
	game_cnt = 0;
	for (std::set<team_game *, team_game_comparator>::iterator it = team_game_history.begin(); it != team_game_history.end(); ++it) {
		int summ_cnt = 0;
		for (int i = 0; i < (*it)->player_count; ++i)
			if (find_summoner((*it)->player_perspectives[i]->summoner_id) != -1)
				summ_cnt++;

		if (summ_cnt >= MIN_TEAMMATES_COUNT) {
			bgt_team_games[find_team((*it)->player_perspectives[0]->summoner_id)].insert((*it));
			game_cnt++;
			//(*it)->print_formatted_desc();
			//putchar('\n');
		}
	}
}

int main()
{
	if (!read_full_game_history()) {
		printf("There is no input file (output_<num>.txt).\n");
		return -1; // Couldn't find the input, aborting
	}

	int bgt_count, game_cnt;
	process_games();
	process_teamgames(bgt_count);


	printf("size of the initial (non-unique) game history: %d\n", game_history.size());
	printf("size of the team (unique) game history: %d\n", team_game_history.size());
	printf("number of BGT games with %d+ members: %d\n", MIN_TEAMMATES_COUNT, bgt_count);
	printf("sizes of teammates normal history:\n");
	bgt_count = 0; game_cnt = 0;
	for (int i = 0; i < NUM_TEAMS; i++) {
		for (int j = 0; j < NUM_TEAM_MEMBERS; j++) {
			for (unsigned int k = 0; k < player_normal_stats[i][j].size(); k++) {
				bgt_count += player_normal_stats[i][j][k].w + player_normal_stats[i][j][k].l;
				game_cnt += player_normal_stats[i][j][k].w + player_normal_stats[i][j][k].l;
			}
			printf("%d\t", game_cnt);
			game_cnt = 0;
		}
		printf("\t%s\n", TEAM_NAMES[i]);
	}
	game_cnt = 0;
	printf("sizes of teammates ranked history:\n");
	for (int i = 0; i < NUM_TEAMS; i++) {
		for (int j = 0; j < NUM_TEAM_MEMBERS; j++) {
			for (unsigned int k = 0; k < player_ranked_stats[i][j].size(); k++) {
				bgt_count += player_ranked_stats[i][j][k].w + player_ranked_stats[i][j][k].l;
				game_cnt += player_ranked_stats[i][j][k].w + player_ranked_stats[i][j][k].l;
			}
			printf("%d\t", game_cnt);
			game_cnt = 0;
		}
		printf("\t%s\n", TEAM_NAMES[i]);
	}
	printf("bgt_count (suma wszystkich) %d\n", bgt_count);
	
	while (true) {
		printf("Which team's history would you like to browse?\n>");
		int team;
		if (!scanf("%d", &team))
			break;
		if (team >= 0 && team < 16) {
			/* Listing all recent team games */
			for (std::set<team_game *, team_game_comparator>::iterator it = bgt_team_games[team].begin(); it != bgt_team_games[team].end(); ++it) {
				int summ_cnt = 0;
				for (int i = 0; i < (*it)->player_count; ++i) {
					if (find_summoner((*it)->player_perspectives[i]->summoner_id) != -1)
						summ_cnt++;

					if (summ_cnt >= MIN_TEAMMATES_COUNT) {
						(*it)->print_formatted_desc();
						putchar('\n');
					}
				}
			}
			/* Listing top recent individual champ for players */
			for (int i = 0; i < NUM_TEAM_MEMBERS; ++i) {
				int j = 0;
				printf("%s\n", SUMMONER_NAMES[team][i]);
				printf("--Ranked--:\n");
				std::sort(player_ranked_stats[team][i].begin(), player_ranked_stats[team][i].end());
				for (std::vector<champ_stats>::reverse_iterator it = player_ranked_stats[team][i].rbegin(); j < 5 && it != player_ranked_stats[team][i].rend(); ++it) {
					printf("%s\t%s%d/%d (%d%%)\n", get_champion_name(it->champion_id), strlen(get_champion_name(it->champion_id)) < 8? "\t": "", it->w, it->l, int(it->winrate()));
					++j;
				}
				j = 0;
				printf("--Normal--:\n");
				std::sort(player_normal_stats[team][i].begin(), player_normal_stats[team][i].end());
				for (std::vector<champ_stats>::reverse_iterator it = player_normal_stats[team][i].rbegin(); j < 3 && it != player_normal_stats[team][i].rend(); ++it) {
					printf("%s\t%s%d/%d (%d%%)\n", get_champion_name(it->champion_id), strlen(get_champion_name(it->champion_id)) < 8? "\t": "", it->w, it->l, int(it->winrate()));
					++j;
				}
				printf("\n");
			}
		}
	}
system("PAUSE");
	return 0;
}

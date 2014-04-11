#ifndef STATIC_DATA_H
#define STATIC_DATA_H

static const int NUM_TEAMS = 16;
static const int NUM_TEAM_MEMBERS = 5;

extern const int TEAMS[NUM_TEAMS][NUM_TEAM_MEMBERS];

extern char const * SUMMONER_NAMES[NUM_TEAMS][NUM_TEAM_MEMBERS];
extern char const * TEAM_NAMES[NUM_TEAMS];
extern char const * SUMMONER_SPELLS[22];

int find_team(int summoner_id);
int find_summoner(int summoner_id);

char * get_champion_name(int champion_id);
#endif
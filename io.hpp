#include <set>
#include "game.hpp"
#include "team_game.hpp"

#ifndef IO_HPP
#define IO_HPP
void read_games_from_file(std::set<game *, game_comparator> &game_history, const char *name);
void read_team_games_from_file(std::set<team_game *, team_game_comparator> &team_game_history, const char *name);
void save_to_file(std::set<game *, game_comparator > &game_history, const char *name);
#endif
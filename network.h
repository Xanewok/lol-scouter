#include <cstdlib>

#ifndef NETWORK_H
#define NETWORK_H

struct MemoryStruct {
	char *memory;
	size_t size;
	MemoryStruct(char *ptr = (char *)malloc(1), int size = 0); // not sure about this tho
	~MemoryStruct();
	void reset();
	
	static size_t write_func( char *ptr, size_t size, size_t nmemb, void *userdata);
};


/* Returns the recent 10 games of the summoner_id by using the api_key
	to communicate with the Riot API in a form of a JSON string.
	(User needs to free() the returned pointer)
	https://developer.riotgames.com/api/methods#!/618/1924
*/
char* get_games(int summoner_id, char *api_key);
char* get_games(CURL *curl, int summoner_id, char *api_key);
#endif
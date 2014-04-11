#include <cstdio>
#include <curl/curl.h>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include "network.h"
#define SINGLE_REQUEST_INTERVAL 1100
#define FULL_REQUEST_INTERVAL 10800000 // 3 hours * 60 minutes * 60 seconds * 1000 milliseconds

MemoryStruct::MemoryStruct(char *ptr, int size) : memory(ptr), size(size) {} // not sure about this tho
MemoryStruct::~MemoryStruct()
{
	if (memory != 0)
		free(memory);
}
void MemoryStruct::reset()
{
	free(memory);
	memory = (char *)malloc(1);
	size = 0;
}

/* http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTWRITEFUNCTION */
size_t MemoryStruct::write_func( char *ptr, size_t size, size_t nmemb, void *userdata)
{
	size_t realsize = size * nmemb;

	MemoryStruct *mem = (MemoryStruct *)userdata;
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


/* Returns the recent 10 games of the summoner_id by using the api_key
	to communicate with the Riot API in a form of a JSON string.
	(User needs to free() the returned pointer)
	https://developer.riotgames.com/api/methods#!/618/1924
*/
char* get_games(CURL *curl, int summoner_id, char *api_key)
{
	assert(curl != 0);

	MemoryStruct chunk;
	char query[200];
	CURLcode res;

	curl_easy_reset(curl);
	if (curl) {
		/* Set the connection options */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, chunk.write_func);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

		sprintf(query, "https://prod.api.pvp.net/api/lol/eune/v1.3/game/by-summoner/%d/recent?api_key=%s", summoner_id, api_key);
		curl_easy_setopt(curl, CURLOPT_URL, query);

		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		else {
			char *ret_str = (char *)malloc(chunk.size + 1);
			strcpy(ret_str, chunk.memory);

			return ret_str;
		}
	}

	return 0;
}

char* get_games(int summoner_id, char *api_key)
{
	char *out_str = 0;

	CURL *curl = curl_easy_init();
	if (curl)
		out_str = get_games(curl, summoner_id, api_key);
	curl_easy_cleanup(curl);

	return out_str;
}
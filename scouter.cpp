#include <cstdio>
#include <set>
#include <curl/curl.h>
#include "cJSON/cJSON.h"
#include "game.hpp"

std::set<game> game_history;

/* Temporary team holder */
const int teams[16][5] ={{19381878, 19739336, 26154202, 29945492, 24423271},/* Random Five */
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

/* http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTWRITEFUNCTION */
size_t write_func( char *ptr, size_t size, size_t nmemb, void *userdata)
{
	printf("%s\n", ptr);
	cJSON *json;
	char *out;

	json = cJSON_Parse(ptr);
	if (!json) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
	} else {
		out = cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}

	return size * nmemb; // return bytes taken care of by func
}

void init()
{
	
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

	const char *test_url = "prod.api.pvp.net/api/lol/eune/v1.4/summoner/";
	char buf[200];
	sprintf(buf, "%s%d,%d,%d,%d,%d?api_key=%s", test_url, teams[0][0], teams[0][1], teams[0][2], teams[0][3], teams[0][4], key);

	curl = curl_easy_init();
	if (curl) {
		/* Tell libcurl to *not* verify the peer. With libcurl you disable this with */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
		
		curl_easy_setopt(curl, CURLOPT_URL, buf);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
							curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();

	return 0;
}
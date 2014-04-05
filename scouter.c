#include <stdio.h>
#include <curl/curl.h>
#include "cJSON/cJSON.h"

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

	return size*nmemb; // return bytes taken care of by func
}

int main(int argc, char* argv[])
{
	printf("This product is not endorsed, certified or otherwise approved in any way by Riot Games, Inc. or any of its affiliates.\n\n"); // we need to put this somewhere else
	printf("Please input your API key: ");
	char key[37];
	scanf("%s", key);

	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_ALL);

	const char *test_url = "prod.api.pvp.net/api/lol/euw/v1.4/summoner/by-name/Blessa?api_key=";
	char buf[200];
	sprintf(buf, "%s%s", test_url, key);



	curl = curl_easy_init();
	if (curl) {
		/* Tell libcurl to *not* verify the peer. With libcurl you disable this with */
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		//curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
	curl_easy_setopt(curl, CURLOPT_URL, buf);

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
		/* example.com is redirected, so we tell libcurl to follow redirection */
		//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

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
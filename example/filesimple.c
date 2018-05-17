#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

char *readJSONFile(){
	FILE *fp = NULL;
	char str[1024];

	fp = fopen("data.json","r");

	if(fp==NULL)
		printf("Error: cannot open");

	while(!feof(fp)){
		fgets(str,1024,fp);
		printf("%s", str);
	}
	fclose(fp);
	return ;
}

//char *JSON_STRING = readJSONFile();


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
	int j;
	char *JSON_STRING = readJSONFile();
	
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	for (i = 1; i < r; i++) {
		if (t[i+1].type != JSMN_ARRAY) {
			continue; /* We expect groups to be an array of strings */
		}
		for (j = 0; j < t[i+1].size; j++) {
			jsmntok_t *g = &t[i+j+2];
			printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
		}
		i += t[i+1].size + 1;
	}
	return EXIT_SUCCESS;
}

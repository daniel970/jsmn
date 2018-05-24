#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */
 static char* json_data = NULL;

 char* readJSONFile() {
   FILE* fp = fopen("data.json","r");
   char cache[256];

   json_data = malloc(sizeof(char));
   int x=0;
   char* pjsondata;
   pjsondata = json_data;

   while(1){
     memset(cache, 0, sizeof(cache));
     fgets(cache, 256, fp);
     if (feof(fp)) break;
     x = strlen(json_data);
     json_data = (char*)realloc(pjsondata, (strlen(cache)+x)*sizeof(char));
     strcat(json_data, cache);
   }
   fclose(fp);
   return json_data;
 }





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
	char* JSON_DATA = (char*)readJSONFile();

	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_DATA, strlen(JSON_DATA), t, sizeof(t)/sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	for (i = 1; i < r; i++){
	if (jsoneq(JSON_DATA, &t[i], "name") == 0) {
		/* We may use strndup() to fetch string value */
		printf("- name: %.*s\n", t[i+1].end-t[i+1].start,
				JSON_DATA + t[i+1].start);
		i++;
	} else if (jsoneq(JSON_DATA, &t[i], "keywords") == 0) {
		/* We may additionally check if the value is either "true" or "false" */
		printf("- keywords: %.*s\n", t[i+1].end-t[i+1].start,
				JSON_DATA + t[i+1].start);
		i++;
	} else if (jsoneq(JSON_DATA, &t[i], "description") == 0) {
		/* We may additionally check if the value is either "true" or "false" */
		printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
				JSON_DATA + t[i+1].start);
		i++;
	} else if (jsoneq(JSON_DATA, &t[i], "examples") == 0) {
    int j;
    printf("- examples:\n");
    if (t[i+1].type != JSMN_ARRAY) {
      continue; /* We expect groups to be an array of strings */
    }
    for (j = 0; j < t[i+1].size; j++) {
      jsmntok_t *g = &t[i+j+2];
      printf("  * %.*s\n", g->end - g->start, JSON_DATA + g->start);
    }
    i += t[i+1].size + 1;
  }


  }
	return EXIT_SUCCESS;
}

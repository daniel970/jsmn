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

void jsonNameList(char* jsonstr, jsmntok_t* t, int tokcount, int counter){
  if(t[tokcount].type == JSMN_STRING && (t[tokcount+1].type == JSMN_STRING || t[tokcount+1].type == JSMN_ARRAY || t[tokcount+1].type == JSMN_OBJECT)){
    printf("[NAME%2d] %.*s\n", counter, t[tokcount].end - t[tokcount].start, jsonstr+t[tokcount].start);
  }
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

  int count =1;

  printf("***** Name List *****\n");
  for(i=1;i<r ;i++){
    jsonNameList(JSON_DATA, t, i, count);
    if(t[i+1].type == JSMN_ARRAY){
      i += t[i+1].size+1;
    }
    else{
      i++;
    }
    count++;
  }

	return EXIT_SUCCESS;
}

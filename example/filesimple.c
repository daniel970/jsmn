#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
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



void jsonNameList(char* jsonstr, jsmntok_t* t, int tokcount, int counter, int* nameTokList){
  if(t[tokcount].type == JSMN_STRING){
    if(t[tokcount+1].type == JSMN_STRING || t[tokcount+1].type == JSMN_ARRAY || t[tokcount+1].type == JSMN_OBJECT){
      nameTokList[counter-1] = tokcount;
    }
  }
}

void printNameList(char* jsonstr, jsmntok_t* t, int counter, int* nameTokList){
  int i = 0;
  for(i=1; i<counter; i++){
    printf("[NAME%2d] %.*s\n", i, t[nameTokList[i-1]].end - t[nameTokList[i-1]].start, jsonstr+t[nameTokList[i-1]].start);
  }
}

void selectNameList(char* jsonstr, jsmntok_t* t, int counter, int* nameTokList){
  int select = -1;
  while(1){
    printf("Select name's no (exit:0): ");
    while (getchar() != '\n'); //to avoid infinite loop
    scanf("%d", &select);

    if(select == 0){
      break;
    }
    else if(select == -1 || select >= counter){
      printf("wrong input\n");
    }
    else{
      printf("[NAME%2d] %.*s\n", select, t[nameTokList[select-1]].end - t[nameTokList[select-1]].start, jsonstr+t[nameTokList[select-1]].start);
    }
  }
}



int main() {
	int i;
	int r;
	int j;
	char* JSON_DATA = (char*)readJSONFile();

	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

  int* nameTokList;
  nameTokList = (int*)malloc(sizeof(int)*100);


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
    jsonNameList(JSON_DATA, t, i, count, nameTokList);

    if(t[i+1].type == JSMN_ARRAY){
      i += t[i+1].size+1;
    }

    else if(t[i].type == JSMN_OBJECT){
    //  if(t[i+1].type == JSMN_OBJECT){
        i += t[i+1].size -1;
        count--;
    //  }
    }
    else{
      i++;
    }
    count++;
  }

  printNameList(JSON_DATA, t, count, nameTokList);
  selectNameList(JSON_DATA, t, count, nameTokList);

	return EXIT_SUCCESS;
}

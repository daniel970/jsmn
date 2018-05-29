#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "../jsmn.h"


typedef struct filedata* pfiledata;
typedef struct filedata{
   int size;
   char* data;
} filedata;


pfiledata readJSONFile() {
   pfiledata data_json;
   FILE* fp = fopen("data.json","r");
   int size = 0;
   while(1){
      char cache[100];
      fgets(cache, 100, fp);
      size = size + strlen(cache);
      if(feof(fp)) break;
   }
   fclose(fp);
   data_json = (pfiledata)malloc(size+sizeof(int));
   data_json->size = size;
   FILE* fp2 = fopen("data.json","r");
   char* JSON_STRING = (char*)malloc(size);
   while(1) {
      char cache[100];
      fgets(cache, 100, fp2);
      if(feof(fp2)) break;
      strcat(JSON_STRING, cache);
   }
   fclose(fp2);
   data_json->data = JSON_STRING;
   free(JSON_STRING);
   return data_json;
}


/*int jsoneq(char *json, jsmntok_t *tok, char *s) {
   if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
         strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
      return 0;
   }
   return -1;
}*/
/*
void jsonNameList(char* jsonstr, jsmntok_t *t, int tokcount, int* nameTokList){
   int i=0;
   int cnt=0;
   char c;
   for(i = 0; i<tokcount; i++){
      c = jsonstr[i];
      switch(c){
         case '{' : case ',':
            for(i++;jsonstr[i]!='\"';i++);
            printf("[NAME%2d] ", ++cnt);
            for(i++;jsonstr[i]!='\"';i++){
               printf("%c",jsonstr[i]);
            }
            printf("\n");
            break;
      }
   }
}
*/
/*
void jsonNameList(char* jsonstr, jsmntok_t* t, int i, int counter){
   int trigger1 = t[i+1].type == JSMN_STRING;
   int trigger2 = t[i+1].type == JSMN_ARRAY;
   int trigger3 = t[i+1].type == JSMN_OBJECT;
   int trigger = trigger1 || trigger2 || trigger3;
   if(t[i].type == JSMN_STRING && trigger) {
      printf("[NAME%2d] %.*s\n", counter, t[i].end - t[i].start, jsonstr+t[i].start);
   }
}*/

//jsonNameList(JSON_STRING, t, i, counter, nameTokList);

void jsonNameList(char* jsonstr, jsmntok_t* t, int i, int count, int *nameTokList){
   //printf("\tout of if loop: i = %d\n", i);
   //printf("\tout of if loop: t[i] type:%d, t[i+1] type:%d\n", t[i].type, t[i+1].type);
   int trigger1 = t[i+1].type == JSMN_STRING;
   int trigger2 = t[i+1].type == JSMN_ARRAY;
   int trigger3 = t[i+1].type == JSMN_OBJECT;
   int trigger = trigger1 || trigger2 || trigger3;
   if(t[i].type == JSMN_STRING && trigger) {
      //printf("\tin of if loop: t[i] type:%d, t[i+1] type:%d\n", t[i].type, t[i+1].type);
      //printf("\tin if loop: i = %d\n", i);
      //printf("\tin if loop: count = %d\n", count);
      nameTokList[count-1] = i;
   }
   else if(t[i].type == JSMN_OBJECT && trigger){
      //printf("in else loop: i = %d\n", i);
      //printf("in else loop: count = %d\n", count);
   }
}

void printNameList(char* jsonstr, jsmntok_t *t, int count, int *nameTokList){
   int j = 0;
   int i = 0;
   printf("***** NAME LIST *******\n");
   for(j = 1; j < count; j++){
      i = nameTokList[j-1];
      //printf("i = %d\n", i);
      printf("[NAME%2d] %.*s\n", j, t[i].end - t[i].start, jsonstr+t[i].start);
   }
}

void selectNameList(char* jsonstr, jsmntok_t *t, int count,int* nameTokList){
   int counter=-1;
   printf("Select name's no(0 to exit): ");
   scanf("%d", &counter);
   if(counter>=count || counter == -1){
      printf("Input error\n");
   } else if(counter == 0){
      return;
   }   else{
      int i = nameTokList[counter-1];
      printf("[NAME%2d] %.*s\n", counter, t[i].end - t[i].start, jsonstr+t[i].start);
      printf("%.*s\n", t[i+1].end - t[i+1].start, jsonstr+t[i+1].start);
   }
}

void jsonObjectList(jsmntok_t* t, int i, int count, int *headTokList) {
   if(t[i].type == JSMN_OBJECT){
      headTokList[count] = i+1;
   }
}

void printObjectList(char* jsonstr, jsmntok_t *t, int headcount, int *headTokList){
   int j = 0;
   int i = 0;
   printf("***** OBJECT LIST *******\n");
   for(j = 1; j < headcount+1; j++){
      i = headTokList[j-1];
      //printf("i = %d\n", i);
      printf("[NAME%2d] %.*s\n", j, t[i+1].end - t[i+1].start, jsonstr+t[i+1].start);
   }
}

void selectObjectList(char* jsonstr, jsmntok_t *t, int count, int* nameTokList, int* headTokList){
   int counter = -1;
   printf("Select name's number(0 to exit): ");
   scanf("%d", &counter);
   if(counter>count || counter == -1){
      printf("Input error\n");
   } else if(counter == 0){
      return;
   }   else{
      int starter = headTokList[counter];
      int ender = 1 + counter*10;
      int j = 1 + (counter-1)*10;
      int i = nameTokList[j-1];
      printf("name: %.*s\n", t[i+1].end - t[i+1].start, jsonstr+t[i+1].start);
      for(j+1; j < ender-1; j++){
         i = nameTokList[j];
         printf("\t[%.*s]: %.*s\n", t[i].end - t[i].start, jsonstr+t[i].start, t[i+1].end - t[i+1].start, jsonstr+t[i+1].start);
      }
   }
}

int main() {
   pfiledata JSON_DATA = readJSONFile();
   printf("Size of data.json : %d\n", JSON_DATA->size);
   int i = 0;
   int r = 0;
   char* JSON_STRING = JSON_DATA->data;
   jsmn_parser p;
   jsmntok_t t[128]; /* We expect no more than 128 tokens */
   int* nameTokList;
   nameTokList = (int*)malloc(sizeof(int)*100);
   jsmn_init(&p);
   r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
   if (r < 0) {
      printf("Failed to parse JSON: %d\n", r);
      return 1;
   }
   if (r < 1 || t[0].type != JSMN_OBJECT) {
      printf("Object expected\n");
      return 1;
   }
   int counter = 1;
   int headTokList[10] = {1,};
   int headcount = 1;
   int size = 0;
   for (i = 1; i < r; i++) {
      //printf("i = %d, counter = %d\n", i, counter);
      jsonNameList(JSON_STRING, t, i, counter, nameTokList);
      //jsonNameList(JSON_STRING, t, i, counter);
      jsonObjectList(t, i, headcount, headTokList);
      if(t[i+1].type == JSMN_ARRAY){
         i += t[i+1].size + 1;
      } else if(t[i].type == JSMN_OBJECT){
         i += t[i+1].size -1;
         counter--;
         headcount++;
      }   else{
         i++;
      }
      counter++;
   }
   printNameList(JSON_STRING, t, counter, nameTokList);
  // printObjectList(JSON_STRING, t, headcount, headTokList);
   selectNameList(JSON_STRING, t, counter, nameTokList);
  // selectObjectList(JSON_STRING, t, headcount, nameTokList, headTokList);
   return EXIT_SUCCESS;
}

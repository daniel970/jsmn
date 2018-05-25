#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

typedef struct filedata* pfiledata;// filedata를 리턴하는 함수를 만들기 위해서 다른 곳에서 filedata를 사용해야 하기 때문에 포인터를 쓴다
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
   }//it makes int size to be the size of every data.json's size
   fclose(fp);
   data_json = (pfiledata)malloc(size+sizeof(int));//makes new array which is size of int size?
   data_json->size = size;// data_json 이라는 filedata에 size 값을 data.json의 size로 만든다
   FILE* fp2 = fopen("data.json","r");
   char* JSON_STRING = (char*)malloc(size);//makes array which is the size of int size
   while(1) {
      char cache[100];
      fgets(cache, 100, fp2);
      if(feof(fp2)) break;
      strcat(JSON_STRING, cache);
   }// add everything in data.json into array JSON_STRING
   fclose(fp2);
   data_json->data = JSON_STRING;//???
   free(JSON_STRING);
   return data_json;
}


int jsoneq(char *json, jsmntok_t *tok, char *s) {
   if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
         strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
      return 0;
   }
   return -1;
}


// readJSONFile()->data; jsmntok_t t[128]; int tokcount;
void jsonNameList(char* jsonstr, jsmntok_t* t, int tokcount, int counter){
   printf("[NAME%2d]\t%.*s\n", counter, t[tokcount].end - t[tokcount].start, jsonstr+t[tokcount].start);
}





int main() {
   pfiledata JSON_DATA = readJSONFile();
   printf("Size of data.json : %d\n", JSON_DATA->size);
   int i;
   int r;
   char* JSON_STRING = JSON_DATA->data;
   jsmn_parser p;
   jsmntok_t t[128]; /* We expect no more than 128 tokens */

   jsmn_init(&p);
   r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));
   if (r < 0) {
      printf("Failed to parse JSON: %d\n", r);
      return 1;
   }

   /* Assume the top-level element is an object */
   if (r < 1 || t[0].type != JSMN_OBJECT) {
      printf("Object expected\n");
      return 1;
   }
   int counter = 1;
   printf("***** NAME LIST *******\n");
   for (i = 1; i < r; i++) {
      jsonNameList(JSON_STRING, &t[i], i+1, counter);
      if(t[i+1].type == JSMN_ARRAY){ //다음 토큰의 타입이 어레이면
         i += t[i+1].size + 1;
      } else{
         i++;
      }
      counter++;
   }
   return EXIT_SUCCESS;
}

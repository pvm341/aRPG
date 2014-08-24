#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csvdata.h"

static char *csv_copy_str = NULL;

void csv_init(char *str){
	char *ip, *op, tp;
  if (NULL !=  csv_copy_str){
    free(csv_copy_str);
  }
  csv_copy_str = (char *) malloc(sizeof(char)*(strlen(str)+1));
//  strcpy(csv_copy_str,str);
  // look for the end of the string
  ip = str;
  op = csv_copy_str;
	while (*ip){
    if (*ip=='\"' || *ip=='\''){
      tp = *ip++;
      while (*ip && *ip != tp){
       *op++ = *ip++;
      }
		} else if (*ip == ','){
      *op++ = 0;
    } else if (*ip == '\n' || *ip == '\r') {
      *op++ =0;
		} else {
      *op++ = *ip;
    }
    ip++;
  }
}

void csv_done(){
  if (NULL != csv_copy_str){
    free(csv_copy_str);
  }
  csv_copy_str = NULL;
}

char *csv_get_data(int n){
  char *p=csv_copy_str, *start;
  start = csv_copy_str;
  while (*p && n){
    while (*p++) /*empty loop */ ;
    n--;
    start=p;
  }
  return start;
}

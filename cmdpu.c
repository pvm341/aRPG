#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdpu.h"
#define CMDLINELEN 80

const char *commands[] = {"north", "east", "south", "west","up", "down","Quit", NULL};

int get_command(const char *commands[], int loc_id, int *(*prompt_func)()){
  char cmdline[CMDLINELEN+1];
  int d, *exits, notfound=1;
  char *ptr;
  do {
    exits=prompt_func(loc_id);
    fgets(cmdline,CMDLINELEN,stdin);
    if ((ptr=strchr(cmdline,'\n'))){
      *ptr=0;
    }
    d=0;
    while ((commands[d] && notfound)){
      if (commands[d]){
        notfound = strcmp(cmdline,commands[d]);
        notfound |= !(exits[d]>=0);
        d+= notfound ?1:0;
      }
    }
  } while (notfound);
  if (d == 6){
    d=-d;
  } else {
    d = (int)exits[d];
  }
  return d;
}

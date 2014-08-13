#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "location.h"
#include "cmdpu.h"
#define CMDLINELEN 80

typedef enum  {NORTH, EAST, SOUTH, WEST, UP, DOWN, HELP, HINT, LOOK, TAKE, LEAVE } CMDS;

const char *commands[] = {"north", "east", "south", "west","up", "down","help",
                          "hint", "look", "take","leave", "Quit", NULL};

int get_command(const char *commands[], int loc_id, int *(*prompt_func)()){
  char cmdline[CMDLINELEN+1];
  int cmd, *exits, notfound=1;
  char *ptr;
  do {
    exits=prompt_func(loc_id);
    fgets(cmdline,CMDLINELEN,stdin);
    if ((ptr=strchr(cmdline,'\n'))){
      *ptr=0;
    }
    if (strcmp(cmdline,"Quit")){
      cmd=0;
      while ((commands[cmd] && notfound)){
        if (commands[cmd]){
          notfound = strcmp(cmdline,commands[cmd]);
          notfound |= !(exits[cmd]>=0);
          cmd+= notfound ?1:0;
        }
      }
      if (notfound == 0 && cmd<6){
        cmd = (int)exits[cmd];
      } else {
        process_cmd(cmd, loc_id);
        notfound=1;
      }
    } else {
      cmd = -1;
      notfound = 0;
    }
  } while (notfound);
  return cmd;
}

void process_cmd(const int cmd, const int cur_loc){
  plocation location = get_loc(cur_loc);
  int i;
  switch(cmd){
    case NORTH:
    case SOUTH:
    case EAST:
    case WEST:
    case UP:
    case DOWN:
      /* nothing to do here at the moment */
      break;
    case HINT:
      fprintf(stdout,"%s\n",location->hint);
      break;

    case HELP:
      fprintf(stdout, "The commands/actions available are :-\n");
      for (i=0;commands[i];i++){
        fprintf(stdout,"%s%s",i>0 && commands[i+1]?", ":!commands[i+1]?" and ":"\0",commands[i]);
      }
      fprintf(stdout,"\n");
      break;
    case LOOK:
      fprintf(stdout,"Look - Not yet implemented\n");
      break;
    case TAKE:
      fprintf(stdout,"Take - Not yet implemented\n");
      break;
    case LEAVE:
      fprintf(stdout,"Leave - Not yet implemented\n");
      break;

  }
}

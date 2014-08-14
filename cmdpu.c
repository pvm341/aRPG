#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "location.h"
#include "cmdpu.h"
#define CMDLINELEN 80

const acommand commands = {
                              {NORTH, "north"},
                              {EAST,  "east"},
                              {SOUTH, "south"},
                              {WEST,  "west"},
                              {UP,    "up"},
                              {DOWN,  "down"},
                              {HELP,  "help"},
                              {HINT,  "hint"},
                              {INVENTORY, "inv"},
                              {LOOK,  "look"},
                              {TAKE,  "take"},
                              {LEAVE, "leave"},
                              {QUIT,  "Quit"},
                              {NORTH, "n"},
                              {NORTH, "go north"},
                              {EAST,  "e"},
                              {EAST,  "go east"},
                              {SOUTH, "s"},
                              {SOUTH, "go south"},
                              {WEST,  "w"},
                              {WEST,  "go west"},
                              {UP,    "u"},
                              {UP,    "go up"},
                              {DOWN,  "d"},
                              {DOWN,  "go down"},
                              {HELP,  "?"},
                              {HINT,  "h"},
                              {INVENTORY, "i"},
                              {INVENTORY, "inventory"},
                              {LOOK,  "l"},
                              {LOOK,  "view"},
                              {LOOK,  "examine"},
                              {TAKE,  "t"},
                              {TAKE,  "get"},
                              {TAKE,  "pickup"},
                              {LEAVE, "p"},
                              {LEAVE, "put"},
                              {LEAVE, "drop"},
                              {QUIT,  "X"},
                              {QUIT,  "Exit"},
                              {END_OF_LIST, "end of list"}
                            };

static int find_command(const acommand commands, int loc_id, int *(*prompt_func)()){
  char cmdline[CMDLINELEN+1];
  int  i,*loc_exits, notfound=1;
  char *ptr;
  CMDS cmd;
  do {
    loc_exits=prompt_func(loc_id);
    fgets(cmdline,CMDLINELEN,stdin);
    if ((ptr=strchr(cmdline,'\n'))){
      *ptr=0;
    }
    i=0;
    while ((commands[i].cmd_no<END_OF_LIST && notfound)){
      notfound = strcmp(cmdline,commands[i].cmdstr);
      notfound |= !(loc_exits[commands[i].cmd_no]>=0);
      if (notfound){
        i++;
      } else {
        cmd = commands[i].cmd_no;
      }
    }
    if (notfound == 0 && cmd<HELP){
      cmd = (int)loc_exits[cmd];
    } else if (notfound == 0){
      notfound = 1;
      if (process_cmd(cmd, loc_id)){
        notfound=0;
        cmd = -1;
      }
    }
  } while (notfound);
  return cmd;
}
int get_command(const int loc_id,int *(*prompt_func)()){
  return find_command(commands,loc_id,prompt_func);
}

int process_cmd(const int cmd, const int cur_loc){
  plocation location = get_loc(cur_loc);
  int i,ret_val=0;
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
      for (i=0;commands[i].cmd_no<END_OF_LIST;i++){
        fprintf(stdout,"%s%s",i>0 && commands[i+1].cmdstr?", ":!commands[i+1].cmdstr?" and ":"\0",commands[i].cmdstr);
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
    case QUIT:
      ret_val = -1;
      break;
  }
  return ret_val;
}

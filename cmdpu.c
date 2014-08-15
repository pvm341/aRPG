#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "location.h"
#include "cmdpu.h"
#define CMDLINELEN 80
#define LOCAL_DEBUG 0

const acommand commands =
    {
      /* first of new commands go below here */
      {NORTH,     "north"},
      {EAST,      "east"},
      {SOUTH,     "south"},
      {WEST,      "west"},
      {UP,        "up"},
      {DOWN,      "down"},
      {HELP,      "help"},
      {HINT,      "hint"},
      {INVENTORY, "inventory"},
      {LOOK,      "look"},
      {TAKE,      "take"},
      {LEAVE,     "leave"},
      {QUIT,      "Quit"},
      {GO,        "go"},
      {SAVE,      "save"},
      {LOAD,      "load"},
      /* first of new commands go above here */
      {NORTH, "n"},
      {EAST,  "e"},
      {SOUTH, "s"},
      {WEST,  "w"},
      {UP,    "u"},
      {DOWN,  "d"},
      {HELP,  "?"},
      {HINT,  "h"},
      {LOOK,  "l"},
      {TAKE,  "t"},
      {LEAVE, "p"},
      {INVENTORY, "i"},
      {INVENTORY, "inv"},
      {LOOK,  "view"},
      {LOOK,  "examine"},
      {TAKE,  "get"},
      {TAKE,  "pickup"},
      {LEAVE, "put"},
      {LEAVE, "drop"},
      {QUIT,  "X"},
      {QUIT,  "Exit"},
      {END_OF_LIST, "end of list"}
                            };

static char *replace_strchr(char *str, char schr, char rchr){
  char *ptr = strchr(str,schr);
  if (ptr){
    *ptr=rchr;
  }
  return str;
}

static CMDS process_cmd_line(char *cmd_str){
  CMDS cmd;
  int idx, notfound;

  cmd_str = replace_strchr(cmd_str, '\n','\0');
#if LOCAL_DEBUG == 1
  printf("**Debug 1 cmd_str <%s>\n",cmd_str?cmd_str:"NULL");
#endif
  cmd_str = replace_strchr(cmd_str, ' ','\0');
#if LOCAL_DEBUG == 1
  printf("**Debug 2 cmd_str <%s>\n",cmd_str?cmd_str:"NULL");
#endif
  // Now cmd_str is just the first word
  notfound = 1;
  idx = 0;
  while (notfound && commands[idx].cmd_no < END_OF_LIST){
    notfound = strcmp(cmd_str,commands[idx].cmdstr);
    if (notfound){
      idx++;
    } else {
      cmd = commands[idx].cmd_no;
    }
  }
  return cmd;
}

static int find_command(const acommand commands, int cur_loc){
  char cmd_line[CMDLINELEN+1];
  int  notfound=1, new_loc;
  char *args;
  CMDS cmd;
  do {
    display_location(cur_loc);
    fgets(cmd_line,CMDLINELEN,stdin);
    cmd = process_cmd_line(cmd_line);
    if (GO == cmd) {
      args = strchr(cmd_line,'\0');
      args++;
      cmd = process_cmd_line(args);
    }
    if (cmd<HELP){
      new_loc = process_cmd(cmd,cur_loc,cmd_line);
      notfound = new_loc== -END_OF_LIST;
    } else if (cmd<END_OF_LIST){
      new_loc = process_cmd(cmd, cur_loc, args);
      notfound = new_loc != -1;
    }
    cur_loc = notfound?cur_loc:new_loc;

  } while (notfound);
  return cur_loc;
}

int process_cmd(int cmd, const int cur_loc, char *cmd_line){
  plocation location = get_loc(cur_loc);
  int i,ret_val=0;
  switch(cmd){
    case QUIT:
      ret_val = -1;
      break;
    case NORTH:
    case SOUTH:
    case EAST:
    case WEST:
    case UP:
    case DOWN:
      ret_val = location->exits[cmd];
      if (ret_val==-1){
        ret_val = -END_OF_LIST;
      }
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

    default:
      fprintf(stdout,"<%s> - Not yet implemented\n",commands[cmd].cmdstr);
      break;
  }
  return ret_val;
}

int get_command(const int loc_id){
  return find_command(commands,loc_id);
}


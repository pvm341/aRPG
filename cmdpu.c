#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "location.h"
#include "cmdpu.h"
#define CMDLINELEN 80
#define LOCAL_DEBUG 1

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
                              {EAST,  "e"},
                              {SOUTH, "s"},
                              {WEST,  "w"},
                              {UP,    "u"},
                              {DOWN,  "d"},
                              {HELP,  "?"},
                              {HINT,  "h"},
                              {LOOK,  "l"},
                              {TAKE,  "t"},
                              {GO,    "go"},
                              {LEAVE, "p"},
                              {SAVE,  "save"},
                              {LOAD,  "load"},
                              {INVENTORY, "i"},
                              {INVENTORY, "inventory"},
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

static CMDS find_cmd_from_line(char *cmd_str){
  char *ptr;
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
  // args is NULL or points to the first character after the space
  // args is returned to the calling routine
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

static int find_command(const acommand commands, int loc_id, int *(*prompt_func)()){
  char cmdline[CMDLINELEN+1];
  int  *loc_exits, notfound=1;
  char *ptr;
  CMDS cmd;
  do {
    loc_exits=prompt_func(loc_id);
    fgets(cmdline,CMDLINELEN,stdin);
#if 0
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
#else
    cmd = find_cmd_from_line(cmdline);
    if (cmd == GO){
      ptr = strchr(cmdline,'\0');
      ptr++;
      cmd = find_cmd_from_line(ptr);
    }
#endif
    if (cmd<HELP){
      notfound = (loc_exits[cmd]<0);
      cmd = (int)loc_exits[cmd];
    } else if (cmd<END_OF_LIST){
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

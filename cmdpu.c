#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "list.h"
#include "location.h"
#include "cmdpu.h"

#define CMDLINELEN 80
#define MAX_REPEATS 999
#define LOCAL_DEBUG 0

const acommand commands =
  {
    /* first of new commands go below here */
    {NORTH,   "north"},
    {EAST,    "east"},
    {SOUTH,   "south"},
    {WEST,    "west"},
    {UP,      "up"},
    {DOWN,    "down"},
    {HELP,    "help"},
    //{HINT,    "hint"},
    {INVENTORY, "inventory"},
    {LOOK,    "look"},
    {TAKE,    "take"},
    {LEAVE,   "leave"},
    {QUIT,    "Quit"},
//    {GO,    "go"},
    {SAVE,    "save"},
    {LOAD,    "load"},
    {MAKE,    "make"},
    {TRADE,   "trade"},
    /* first of new commands go above here */
    /* synonims of can be placed below here */
    {NORTH, "n"},
    {EAST,  "e"},
    {SOUTH, "s"},
    {WEST,  "w"},
    {UP,    "u"},
    {DOWN,  "d"},
    {HELP,  "?"},
    //{HINT,  "h"},
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
    /* this must be the last line of commands */
    {END_OF_CMDS, NULL}
  };

static char *replace_strchr(char *str, char schr, char rchr){
  char *ptr = strchr(str,schr);
  if (ptr){
    *ptr=rchr;
  }
  return str;
}

static void unknown_cmd(FILE *file,char *s){
  fprintf(file,"<");
  if (file == stdout){
    fprintf(file,"%c[35m",27);
  }
  fprintf(file,"%s",s);
  if (file == stdout){
    fprintf(file,"%c[39m",27);
  }
  fprintf(file,"> - not a valid command! help displays the list\n");
}

static void notreadyyet(FILE *file, CMDS cmd){
  fprintf(file,"<");
  if (file == stdout){
    fprintf(file,"%c[36m",27);
  }
  fprintf(file, "%s",commands[cmd].cmdstr);
  if (file == stdout){
    fprintf(file,"%c[39m",27);
  }
  fprintf(file,"> - Not yet implemented\n");
}

static int find_cmd(char *cmd_str){
  int idx, notfound;

  cmd_str = replace_strchr(cmd_str, '\n','\0');
#if LOCAL_DEBUG == 2
  printf("**Debug 1 cmd_str <%s>\n",cmd_str?cmd_str:"NULL");
#endif
  cmd_str = replace_strchr(cmd_str, ' ','\0');
#if LOCAL_DEBUG == 2
  printf("**Debug 2 cmd_str <%s>\n",cmd_str?cmd_str:"NULL");
#endif
  // Now cmd_str is just the first word
  notfound = 1;
  idx = -1;
  while (notfound){
    idx++;
    notfound = !(NULL == commands[idx].cmdstr) &&
     (strcmp(cmd_str,commands[idx].cmdstr));
  }
#if LOCAL_DEBUG == 2
  printf("**Debug 3 cmd <%s>\n",commands[idx].cmdstr);
#endif
  return idx;
}

void game_loop(){
  char cmd_line[CMDLINELEN+1];
  int  notfound=1, game_quit=0, i, idx, cur_loc = 0, new_loc, repeats;
  char *args,*cmd_ptr, *separator, *separators=" ;";
  CMDS cmd;

  do {
    display_location(cur_loc);
    fgets(cmd_line,CMDLINELEN,stdin);

    if (isdigit(*cmd_line)){
      // command not found so assume not found
      notfound = 1;
      // may be a command preceeded by a number indicating repeats
      i = 0;
      repeats = 0;
      while (strlen(cmd_line)>i && isdigit(cmd_line[i]) && repeats<MAX_REPEATS){
        repeats *= 10;
        repeats += (cmd_line[i++] - '0');
      }
      while(strlen(cmd_line)>i && isdigit(cmd_line[i])){
        i++;
      }
      if (repeats){
        if (repeats>MAX_REPEATS)
          repeats = MAX_REPEATS;
        idx = find_cmd(cmd_line+i);
        cmd = commands[idx].cmd_no;
        if (HELP > cmd) {
          notfound = 0;
          new_loc = cur_loc;
          while (repeats && new_loc>=0) {
            new_loc = process_cmd(cmd,cur_loc,cmd_line+i);
            // remain in place if the exit in repeated direction is invalid
            cur_loc = new_loc>=0?new_loc:cur_loc;
            repeats--;
          }
          if (new_loc<=0)
            new_loc = cur_loc;
        }
      }
    } else {
      idx = find_cmd(cmd_line);
      cmd = commands[idx].cmd_no;
//    if (GO == cmd) {
//      args = strchr(cmd_line,'\0');
//      args++;
//      cmd = process_cmd_line(args);
//    }
      if (HELP>cmd){
        new_loc = process_cmd(cmd,cur_loc,cmd_line);
        notfound = new_loc == -END_OF_CMDS;
        if (notfound){
          fprintf(stdout, "You can not go %s as there is no exit"
                          " in that direction\n",commands[cmd].cmdstr);
        }
      } else if (QUIT == abs(cmd)) {
        // found QUIT command
        cmd = process_cmd(cmd,cur_loc,cmd_line);
        game_quit = QUIT == cmd;
      } else if (cmd<END_OF_CMDS){
        new_loc = process_cmd(cmd, cur_loc, args);
        notfound = new_loc != -1;
      } else {
        unknown_cmd(stdout, cmd_line);
      }
    }
    cur_loc = notfound?cur_loc:new_loc;
  } while (!game_quit);
}

/* Runs a command. Returns 0 if it succeeded. Returns 1 if it failed */

int parse_cmd(char *cmd_line)
{
  return 0;
}

/* Cleans the command line input.*/

int clean_cmd_line(char *command)
{
  // remove new line characters
  // remove \0 characters
  return 0;
}


int process_cmd(int cmd, const int cur_loc, char *cmd_line_args){
  plocation location = get_loc(cur_loc);
  int i,ret_val=-END_OF_CMDS;

  switch(cmd){
    case QUIT:
      fprintf(stdout,"Quit game are you sure ? (yes or no)\n");
      fgets(cmd_line_args,10,stdin);
      replace_strchr(cmd_line_args,'\n','\0');
      ret_val = strcmp(cmd_line_args,"yes")?-QUIT:QUIT;
      break;
    case NORTH:
    case SOUTH:
    case EAST:
    case WEST:
    case UP:
    case DOWN:
      ret_val = location->exits[cmd];
      if (ret_val==-1){
        ret_val = -END_OF_CMDS;
      }
    break;
//    case HINT:
//    fprintf(stdout,"%s\n",location->hint);
//    break;
  case HELP:
    fprintf(stdout, "The commands/actions available are :-\n");
    for (i=0;commands[i].cmd_no<END_OF_CMDS;i++){
      fprintf(stdout,"%s",i>0 && commands[i+1].cmdstr?", ":!commands[i+1].cmdstr?" and ":"\0");
      fprintf(stdout,"%c[33m%s%c[39m",27,commands[i].cmdstr,27);
    }
    fprintf(stdout,"\n");
    break;

  default:
    notreadyyet(stdout,cmd);
    break;
  }
  return ret_val;
}

int get_direction_from_string(char *dir_str){
  int idx = find_cmd(dir_str);
  CMDS cmd = commands[idx].cmd_no;
  if (cmd<NORTH || cmd>DOWN ){
    cmd = END_OF_CMDS;
  }
  return cmd;
}

/*
else if (NULL != (separator = strpbrk(cmd_line,separators)){
        cmd_ptr = cmd_line;
        while (separator){
          args = separator+1;
          replace_strchr(cmd_ptr,*separator,'\0');
          // now cmd_line is just the command and args now rest of the cmd_line
        idx = process_cmd_line(cmd_ptr);


      }
*/

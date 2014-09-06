#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "list.h"
#include "location.h"
#include "cmdpu.h"
#include "ansiutils.h"

#define CMDLINELEN 80
#define MAX_REPEATS 999
#define LOCAL_DEBUG 0

extern FILE *nul;
extern const char *colours[];
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

static void unknown_cmd(FILE *file,char *s){
  ansi_fprintf(stdout,"<[YELLOW,BLUE]%s[RESET,RESET]> - "
        "not a valid command! help displays the list\n",s);
}

static void notreadyyet(FILE *file, CMDS cmd){
  ansi_fprintf(stdout,"<[BLACK,CYAN]%s[RESET,RESET]>- Not yet implemented\n",
        commands[cmd].cmdstr);
}

static int find_cmd(char *cmd_str){
  int idx, notfound;

  cmd_str = replace_strchr(cmd_str, '\n','\0');
  cmd_str = replace_strchr(cmd_str, ' ','\0');
  // Now cmd_str is just the first word
  notfound = 1;
  idx = -1;
  while (notfound){
    idx++;
    notfound = !(NULL == commands[idx].cmdstr) &&
     (strcmp(cmd_str,commands[idx].cmdstr));
  }
  return idx;
}

void game_loop(){
  // main game centre
  // the command line
  char cmd_line[CMDLINELEN+1];
  int  notfound=1, game_quit=0, i, idx, cur_loc = 1, new_loc, repeats;
  char *args, *cmd_ptr, *separator, *separators=" ;";
  CMDS cmd;

  do {
    display_location(cur_loc);
    fgets(cmd_line,CMDLINELEN,stdin);

    // check for repeated direction to travel
    // 5n or 23up 430west
    // is the first character of cmd_line a digit okay so starting of a
    // repeated command (direction) to travel
    if (isdigit(*cmd_line)){
      // for safety set not found as no idea at present what follows the digit
      notfound = 1;
      // using i as index to the cmd_line string set i to zero as well repeats
      i = 0;
      repeats = 0;
      // now step through the cmd_line each time multiply number of repeats by
      // and adding the value of the next digit
      while (strlen(cmd_line)>i && isdigit(cmd_line[i]) && repeats<MAX_REPEATS){
        repeats *= 10;
        repeats += (cmd_line[i++] - '0');
      }
      // may have cut off early because a very long string of digit made
      // repeats bigger than predefined value
      // so if there are digits remaining clear them
      while(strlen(cmd_line)>i && isdigit(cmd_line[i])){
        i++;
      }
      // no good of doing the next bit if repeats are zero
      if (repeats){
        // just make sure there are not more than max repeats
        if (repeats>MAX_REPEATS)
          repeats = MAX_REPEATS;
        // now find the command probably a direction
        idx = find_cmd(cmd_line+i);
        cmd = commands[idx].cmd_no;
        //make sure it is a direction if cmd < HELP it is a direction
        if (HELP > cmd) {
          // assume that the exit is good in the chosen direction
          notfound = 0;
          // just to force the start of the while copy cur_loc in to new_loc
          new_loc = cur_loc;
          while (repeats && 0<=new_loc) {
            new_loc = process_cmd(cmd,cur_loc,cmd_line+i);
            // remain in place if the exit in repeated direction is invalid
            cur_loc = 0<new_loc?new_loc:cur_loc;
            repeats--;
          }
          // this is a bug fix so there is no seg fault and ensures remain in
          // place if the new loc is not valid.
          if (0 == new_loc)
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
          ansi_fprintf(stdout, "You can not go [YELLOW,RED]%s[RESET,RESET] as "
             "there is no exit in that direction\n",commands[cmd].cmdstr);
        }
      } else if (QUIT == abs(cmd)) {
        // found QUIT command
        cmd = process_cmd(cmd,cur_loc,cmd_line);
        game_quit = QUIT == cmd;
      } else if (END_OF_CMDS > cmd){
        new_loc = process_cmd(cmd, cur_loc, args);
        notfound = 0 >= new_loc ;
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
  char *err;

  switch(cmd){
    case QUIT:
      ansi_fprintf(stdout,"Quit game are you sure ? [YELLOW,BLUE]"
        "[yes or no][RESET,RESET]\n");
      err = fgets(cmd_line_args,10,stdin);
      if (NULL != err) {
        replace_strchr(cmd_line_args,'\n','\0');
        ret_val = strcmp(cmd_line_args,"yes")?-QUIT:QUIT;
      }
      break;
    case NORTH:
    case SOUTH:
    case EAST:
    case WEST:
    case UP:
    case DOWN:
      ret_val = location->exits[cmd];
      if (0 >= ret_val ){
        ret_val = -END_OF_CMDS;
      }
    break;
//    case HINT:
//    fprintf(stdout,"%s\n",location->hint);
//    break;
  case HELP:
    fprintf(stdout, "The commands/actions available are :-\n");
    for (i=0;commands[i].cmd_no<END_OF_CMDS;i++){
        ansi_fprintf(stdout,"%s[YELLOW]%s[RESET]",i>0 && commands[i+1].cmdstr
                ?", ":!commands[i+1].cmdstr?" and ":"\0",commands[i].cmdstr);
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


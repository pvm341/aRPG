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
      //{HINT,      "hint"},
      {INVENTORY, "inventory"},
      {LOOK,      "look"},
      {TAKE,      "take"},
      {LEAVE,     "leave"},
      {QUIT,      "Quit"},
//      {GO,        "go"},
      {SAVE,      "save"},
      {LOAD,      "load"},
      {MAKE,      "make"},
      {TRADE,     "trade"},

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

static int process_cmd_line(char *cmd_str){
//  CMDS cmd = END_OF_LIST;
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
    notfound = !(NULL == commands[idx].cmdstr) && (strcmp(cmd_str,commands[idx].cmdstr));
  }
#if LOCAL_DEBUG == 2
  printf("**Debug 3 cmd <%s>\n",commands[idx].cmdstr);
#endif
  return idx;
}

static int find_command(const acommand commands, int cur_loc){
  char cmd_line[CMDLINELEN+1];
  int  notfound=1, idx, new_loc;
  char *args;
  CMDS cmd;
  do {
    display_location(cur_loc);
    fgets(cmd_line,CMDLINELEN,stdin);

    idx = process_cmd_line(cmd_line);
    if (END_OF_CMDS == idx){
      notfound = 1;
    } else {
      cmd = commands[idx].cmd_no;
//      if (GO == cmd) {
//        args = strchr(cmd_line,'\0');
//        args++;
//        cmd = process_cmd_line(args);
//      }
      if (cmd<HELP){
        new_loc = process_cmd(cmd,cur_loc,cmd_line);
        notfound = new_loc == -END_OF_CMDS;
      } else if (cmd<END_OF_CMDS){
        new_loc = process_cmd(cmd, cur_loc, args);
        notfound = new_loc != -1;
      } else {
        unknown_cmd(stdout, cmd_line);
      }
    }
    cur_loc = notfound?cur_loc:new_loc;
  } while (notfound);
  return cur_loc;
}

static int game_loop(const acommand commands, int cur_loc){
    char cmd_line[CMDLINELEN+1];
    int  running=1, idx, new_loc;
    char *args;
    CMDS cmd;
    do {
        /* Display the current location before and after user input has been taken */
        display_location(cur_loc);
        /* Get the user input */
        fgets(cmd_line,CMDLINELEN,stdin);
        /* Clean the command for parsing */
        clean_cmd_line(cmd_line);
        /* Parse the command, an example: w;e;s;2w;u;dd2w; */
        
        /* Handle quit command logic here. This is to simplify the message passing for the game loop. */

        if(cmd_line == "quit"){
            // todo: ask if the user is sure
            printf("Are you sure? Type yes or no\n");
            fgets(cmd_line,CMDLINELEN,stdin);
            clean_cmd_line(cmd_line);
            if(cmd_line == "yes"){
                running = 0;
                break;
            } else if (cmd_line == "no"){
                display_location(cur_loc);
                fgets(cmd_line,CMDLINELEN,stdin);
                clean_cmd_line(cmd_line);
            }
        }

        /* Check for standard word notation with no parameters, such as: north */
        
        if(find_command(cmd_line)){
            process_cmd(cmd_line);
        } else if (find_command_with_parameters(cmd_line)){
            /* Check for standard word notation with parameters, such as: look west, go north, or scan east */
            // todo: implement detection and parsing off this

        } else {

            /* Check for speed walking notations if the basic notations have not been found */
            for(int index = 0; index < strlen(cmd_line); index++)
            {
                /*  Check for standard word with semicolon notation, such as: north;north;look;west; */
                // todo: this needs a better implementation, mine does not work!
                for(int j = 0, accum = 0;j < strlen(cmd_line); j++)
                {
                    accum++;
                    if(cmd_line[j] == ";"){
                        process_cmd(cmd_line[j]);
                        accum = 0;
                    }
                }

                /* Check for number command notation, such as: 5n */
                if(isdigit(cmd_line[index])){
                    for(int j = 0; j < cmd_line[index]; j++)
                        process_cmd(cmd_line[index+1]);
                    index++;
                }

                /* Check for semicolon command notation, such as: w;n;s;e;e;w; */
                if(cmd_line[index] == ";"){
                    // skip it
                    index++;
                }

                /* Check for single character movement notation */
                // commentary: probably this check is unnecessary
                int tmp = cmd_line[index];
                if(tmp == "n" || tmp == "e" || tmp == "s" || tmp == "w" || tmp == "u" || tmp == "d"){

                }

                if(isalpha(cmd_line[index])){
                    process_cmd(cmd_line[index+1]);
                }
            }
        }
    } while (running);
    return cur_loc;
}

int clean_cmd_line(char * command)
{
    // remove new line characters
    // remove \0 characters 
}




int process_cmd(int cmd, const int cur_loc, char *cmd_line){
  plocation location = get_loc(cur_loc);
  int i,ret_val=-END_OF_CMDS;

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
        ret_val = -END_OF_CMDS;
      }
      break;
//    case HINT:
//      fprintf(stdout,"%s\n",location->hint);
//      break;
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

int get_command(const int loc_id){
  return find_command(commands,loc_id);
}


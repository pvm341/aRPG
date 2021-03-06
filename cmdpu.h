#ifndef CMDPU_H
#define CMDPU_H

typedef enum  {
                NORTH, EAST, SOUTH, WEST, UP, DOWN, HELP,
                /*HINT,*/ INVENTORY, LOOK, TAKE, LEAVE, QUIT,
                /*GO, */
                SAVE, LOAD,
                MAKE, TRADE,
                END_OF_CMDS=256
              } CMDS;

struct scommand {
  CMDS cmd_no;
  char *cmdstr;
};

typedef struct scommand tcommand, acommand[], *pcommand, **dcommand;
const acommand commands;
int get_command(int loc_id);
int process_cmd(int cmd, const int cur_loc, char *args);
int get_direction_from_string(char *dir_str);
void game_loop();
#endif

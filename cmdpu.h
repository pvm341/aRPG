#ifndef CMDPU_H
#define CMDPU_H

typedef enum   {NORTH, EAST, SOUTH, WEST, UP, DOWN, HELP,
                HINT, INVENTORY, LOOK, TAKE, LEAVE, QUIT,
                /*GO, */
                SAVE, LOAD,
                END_OF_LIST } CMDS;

struct scommand {
  CMDS cmd_no;
  char *cmdstr;
};

typedef struct scommand tcommand, acommand[], *pcommand, **dcommand;

int get_command(int loc_id);
int process_cmd(int cmd, const int cur_loc, char *args);
#endif

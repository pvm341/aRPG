#ifndef CMDPU_H
#define CMDPU_H

typedef enum   {NORTH, EAST, SOUTH, WEST, UP, DOWN, HELP,
                HINT, INVENTORY, LOOK, TAKE, LEAVE, QUIT,
                END_OF_LIST } CMDS;

struct scommand {
  CMDS cmd_no;
  char *cmdstr;
};

typedef struct scommand tcommand, acommand[], *pcommand, **dcommand;

extern const acommand commands;

int get_command(int loc_id,int *(*prompt_func)());
int process_cmd(const int cmd, const int cur_loc);
#endif

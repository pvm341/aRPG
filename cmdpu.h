#ifndef CMDPU_H
#define CMDPU_H

extern const char *commands[];

int get_command(const char *commands[], int loc_id, int *(*prompt_func)());
void process_cmd(const int cmd, const int cur_loc);
#endif

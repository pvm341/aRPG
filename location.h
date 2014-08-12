#ifndef LOCATIONS_H
#define LOCATIONS_H

struct slocation {
int id, exits[6];
char description[1024];
pll items_list;
};

typedef struct slocation tlocation, *plocation,  alocation[], **dlocation;

int get_new_loc(const int loc_id);
#endif

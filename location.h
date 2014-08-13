#ifndef LOCATIONS_H
#define LOCATIONS_H

struct slocation {
int id, exits[6];
char description[1024];
pll items_list;
};

typedef struct slocation tlocation, *plocation,  alocation[], **dlocation;

int *display_location(const int loc_id);
#endif

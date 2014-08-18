#ifndef LOCATIONS_H
#define LOCATIONS_H

struct slocation {
int id, exits[6];
char description[1024];
char hint[141];
pll items_list;
};

typedef struct slocation tlocation, *plocation,  alocation[], **dlocation;

extern alocation dummy;


void display_location(const int loc_id);
plocation get_loc(const int loc_id);
void add_location(tlocation a_new_location);
void armageddon();
void genesis();
#endif

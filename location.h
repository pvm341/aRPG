#ifndef LOCATIONS_H
#define LOCATIONS_H

#define MAX_LOC_NAME_SIZE 21
#define MAX_LOC_DESC_SIZE 768
struct slocation {
  int id, exits[6];
  char name[MAX_LOC_NAME_SIZE];
  char description[MAX_LOC_DESC_SIZE];
  pll items_list;
  pll players;
  pll monsters;
};

typedef struct slocation tlocation, *plocation,  alocation[], **dlocation;

void display_location(const int loc_id);
plocation get_loc(const int loc_id);
void add_location(tlocation a_new_location);
void armageddon();
void genesis();
void load_the_world(char *worldname);
#endif

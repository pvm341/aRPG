#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "cmdpu.h"
#include "location.h"
#include "player.h"
#include "csvdata.h"

#define DEFAULT_FILE "default-world.csv"
#define DEBUG 1

extern acommand commands;
typedef _Bool boolean;

static pll the_world;

static int cmp_loc_id(const plocation item1, const int *item2){
  return item1->id - *item2;
}

static int cmp_loc_name(const plocation item1, const char *item2){
  return strcmp(item1->name,item2);
}

static int find_highest_id(){
  pll ptr;
  int id = 0;
  for (ptr = the_world; ptr; ptr = ptr->next){
    plocation loc = ptr->data;
    if (id< loc->id){
      id = loc->id;
    }
  }
  return id+1;
}

static int opposite_direction (const int current_direction){
  int ret_val = -1;
  switch (current_direction){
    case NORTH:
      ret_val = SOUTH;
      break;
    case EAST:
      ret_val = WEST;
      break;
    case SOUTH:
      ret_val = NORTH;
      break;
    case WEST:
      ret_val = EAST;
      break;
    case UP:
      ret_val = DOWN;
      break;
    case DOWN:
      ret_val = UP;
      break;
    }
  return ret_val;
}

static int link_locations(const int id1, const int id2, const int link1to2){
  pll loc1, loc2;
  int error = 1;
  loc1 = find_item(the_world,&id1,cmp_loc_id);
  loc2 = find_item(the_world,&id2,cmp_loc_id);
  if (NULL != loc1 && NULL != loc2){
    plocation l1 = loc1->data, l2 = loc2->data;
    l1->exits[link1to2] = id2;
    l2->exits[opposite_direction(link1to2)] = id1;
    error = 0;
  }
  return error;
}

static plocation find_link_by_name(const char *name){
  pll record = find_item(the_world,name,cmp_loc_name);
  return NULL == record?NULL:record->data;
}

static int link_by_names(const char *name1,
                         const char *name2,
                         const int link1to2){
  pll rec1, rec2;
  int error = 1;
  rec1 = find_item(the_world,name1,cmp_loc_name);
  rec2 = find_item(the_world,name2,cmp_loc_name);
  if (NULL != rec1 && NULL != rec2){
    plocation l1 = rec1->data, l2 = rec2->data;
    l1->exits[link1to2] = l2->id;
    l2->exits[opposite_direction(link1to2)] = l1->id;
    error = 0;
  }
  return error;
}

static void add_location_to_world(dll the_world,plocation new_location){
  add_list_node(the_world,new_location);
}

void add_location(tlocation a_new_location){
  plocation new_location = (plocation)malloc(sizeof(tlocation));
  if (NULL != new_location){
    memcpy(new_location,&a_new_location,sizeof(tlocation));
    add_location_to_world(&the_world,new_location);
  }
}

void display_location(const int loc_id){
  int c, d;
  plocation the_location = get_loc(loc_id);
  if (the_location){
    fprintf(stdout,"%s\n",the_location->description);
    fprintf(stdout,"Exits available are (");
    for(d=NORTH, c=0; d<=DOWN; d++ ){
      if (the_location->exits[d]>=0) {
        if (c++){
          fprintf(stdout,",");
        }
        fprintf(stdout,"%s",commands[d].cmdstr);
      }
    }
    fprintf(stdout,")\nWhat to do ?");
  }
}

plocation get_loc(const int loc_id){
  pll found_loc = find_item(the_world,&loc_id,cmp_loc_id);
  return found_loc->data;
}

void genesis(){
  load_the_world(DEFAULT_FILE);
}

void save_the_world(){

}


void load_the_world(char *worldname){
  FILE *csv;
  tlocation loc;
  plocation ploc;
  char line[1200];
  int line_number=0,set_start = 0, r;

  for (int n=NORTH; n<=DOWN; n++){
    loc.exits[n] = -1;
  }
  if (NULL != (csv=fopen(worldname,"r"))){
    while (!feof(csv)){
      fgets(line,1199,csv);
      line_number++;
      if (*line != '#'){
        csv_init(line);
        char name1[21],name2[21],direction[6];
        sscanf(csv_get_data(0)," %d",&r);
        switch(r){
          case 0:
            // allows only one location to be the start point for every player
            // once successfully set, subsequent CSV records of type zero
            // are ineffective
            if (!set_start){
              strcpy(name1,csv_get_data(1));
              ploc = find_link_by_name(name1);
              if (NULL != ploc){
                int old_id = ploc->id;
                ploc->id = 0;
                set_start = line_number;
                for (pll tmp_ptr = the_world; tmp_ptr; tmp_ptr = tmp_ptr->next){
                  ploc = tmp_ptr->data;
                  for(int dir=NORTH;dir<=DOWN;dir++){
                    if (ploc->exits[dir] == old_id)
                      ploc->exits[dir] = 0;
                  }
                }
              }
            } else {
              fprintf(stderr,"Attempt to reassign start location at %d of %s"
                " originally set in line %d\n",
                line_number, worldname, set_start);
            }
            break;
          case 1:
            strcpy(loc.name,csv_get_data(1));
            strcpy(loc.description,csv_get_data(2));
            loc.id = find_highest_id();
#if DEBUG == 1
            fprintf(stderr,"name:%s\ndescription:%s\n",loc.name,loc.description);
#endif
            add_location(loc);
            break;
          case 2:
            strcpy(name1,csv_get_data(1));
            strcpy(name2,csv_get_data(2));
            strcpy(direction,csv_get_data(3));
            link_by_names(name1,name2,get_direction_from_string(direction));
            break;
        }
      }
    }
    fclose(csv);
  }
}

void armageddon(){
  del_all_players();
  delete_all(&the_world,free);
}

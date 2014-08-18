#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "cmdpu.h"
#include "location.h"
#include "player.h"

extern acommand commands;

static pll the_world;

static int cmp_loc_id(const plocation item1, const int *item2){
  return item1->id - *item2;
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
  tlocation loc;
  loc.items_list=NULL;
  strcpy(loc.description,"Ground floor of Central Building");
  strcpy(loc.name,"CentralBuildingGF");
  loc.monsters = NULL;
  loc.players = NULL;
  loc.id = find_highest_id();
  loc.exits[NORTH]=-1;
  loc.exits[EAST]=-1;
  loc.exits[SOUTH]=-1;
  loc.exits[WEST]=-1;
  loc.exits[DOWN]=-1;
  loc.exits[UP]=-1;
  add_location(loc);
  loc.id = find_highest_id();
  strcpy(loc.description,"An entrance to a dark cave");
  strcpy(loc.name,"CaveEntrance");
  add_location(loc);
  link_locations(1,2,NORTH);
  loc.id = find_highest_id();
  strcpy(loc.description,"Dark of a cave");
  strcpy(loc.name,"DarkCave");
  add_location(loc);
  link_locations(2,3,NORTH);
  loc.id = find_highest_id();
  strcpy(loc.description,"A forest of evergreen");
  strcpy(loc.name,"EverGreenForest");
  add_location(loc);
  link_locations(1,4,EAST);
  loc.id = find_highest_id();
  strcpy(loc.description,"A sandy beach on a fresh water lake");
  strcpy(loc.name,"SandyBeach");
  add_location(loc);
  link_locations(1,5,SOUTH);
  loc.id = find_highest_id();
  strcpy(loc.description,"Eastern river bank, a rope bridge is available to "
                         "cross to the West side");
  strcpy(loc.name,"EastRiverBank");
  add_location(loc);
  link_locations(1,6,WEST);
  loc.id = find_highest_id();
  strcpy(loc.description,"First floor of the central building");
  strcpy(loc.name,"CentralBuilding1F");
  add_location(loc);
  link_locations(1,7,UP);
  loc.id = find_highest_id();
  strcpy(loc.description,"Basement of the central building");
  strcpy(loc.name,"CentralBuildingBa");
  add_location(loc);
  link_locations(1,8,DOWN);
  loc.id = find_highest_id();
  strcpy(loc.description,"Western river bank, a rope bridge is available to "
                         "cross to the East side");
  strcpy(loc.name,"WestRiverBank");
  add_location(loc);
  link_locations(9,6,EAST);
}

void save_the_world(){
}

void load_the_world(){
}

void armageddon(){
  del_all_players();
  delete_all(&the_world,free);
}

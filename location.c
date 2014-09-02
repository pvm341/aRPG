#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "cmdpu.h"
#include "location.h"
#include "player.h"
#include "csvdata.h"
#include "ansiutils.h"

#define DEFAULT_FILE "default-world.csv"
#define DEBUG 0
#define CSV_MAX_LINE_LENGTH 1200
#define CSV_LOCATION_NAME_LENGTH 21
#define CSV_DIRECTION_LENGTH 6
#define CSV_EXIT_LENGTH 10

extern acommand commands;
typedef _Bool boolean;

static pll the_world;

/* 
 * Helper Functions 
 */
static int cmp_loc_id(const plocation item1, const int *item2){
  return item1->id - *item2;
}

static int cmp_loc_name(const plocation item1, const char *item2){
  return strcmp(item1->name,item2);
}

static int find_highest_id(void){
  pll ptr;
  int id = 1; // one is reserved for start location

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

#if 0
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
#endif

static plocation find_link_by_name(const char *name){
  pll record = find_item(the_world,name,cmp_loc_name);
  return NULL == record?NULL:record->data;
}

static int link_by_names(const char *name1,
                         const char *name2,
                         const int link1to2,
                         const int link_type){
  pll rec1, rec2;
  int error = 1;
  rec1 = find_item(the_world,name1,cmp_loc_name);
  rec2 = find_item(the_world,name2,cmp_loc_name);
  if (NULL != rec1 && NULL != rec2){
    plocation l1 = rec1->data, l2 = rec2->data;
    switch (link_type){
      case 3: // both in and out exit
        l1->exits[link1to2] = l2->id;
        l2->exits[opposite_direction(link1to2)] = l1->id;
        error = 0;
        break;
      case 2: // exit only
        l1->exits[link1to2] = l2->id;
        l2->exits[opposite_direction(link1to2)] = -l2->id;
        error = 0;
        break;
      case 1: // entry only
        l1->exits[link1to2] = -l1->id;
        l2->exits[opposite_direction(link1to2)] = l1->id;
        error = 0;
        break;
    }
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
      if (0 != the_location->exits[d]) {
        if (c++){
          fprintf(stdout,",");
        }
        if (0 < the_location->exits[d]){
          fprintf(stdout,"%s",commands[d].cmdstr);
        } else {
          ansi_fprintf(stdout,"[MAGENTA]%s[RESET]",commands[d].cmdstr);
        }
      }
    }
    fprintf(stdout,")\nWhat to do ?");
  }
}

plocation get_loc(const int loc_id){
  pll found_loc = find_item(the_world,&loc_id,cmp_loc_id);
  return found_loc->data;
}

void genesis(void){
  load_the_world(DEFAULT_FILE);
}

void save_the_world(void){
    printf("There is no hope!");

}


void load_the_world(char *worldname){
  FILE *csv;
  tlocation loc;
  plocation ploc;
  char line[CSV_MAX_LINE_LENGTH];
  int line_number=0,set_start = 0, r;
  /* Set all exits to initially closed */
  for (int n=NORTH; n<=DOWN; n++){
    loc.exits[n] = 0;
  }
  if (NULL != (csv=fopen(worldname,"r"))){ /* Open the file */
    while (!feof(csv)){ /* While not end of file */
      /* 
       * fgets used instead of gets to stop bufferoverflow security risk.
       * it adds a '\0' anyway irrespective of length, so:
       * CSV_MAX_LINE_LENGTH-1 to be safe.
       */
      fgets(line,CSV_MAX_LINE_LENGTH-1,csv); /* Get line from the csv and put it in the string buffer */
      line_number++;
      if (*line != '#'){ /* If the line is not a comment */
        csv_init(line); /* Parses CSV line into memory */ 
        char name1[CSV_LOCATION_NAME_LENGTH];
        char name2[CSV_LOCATION_NAME_LENGTH];
        char direction[CSV_DIRECTION_LENGTH],exit_type[CSV_EXIT_LENGTH];
        int link_type;

        /* Determine what type of csv entry the line is */
        sscanf(csv_get_data(0)," %d",&r);
        switch(r){

          case 0:
            /* Spawn location for the player */

            if (!set_start){
              strcpy(name1,csv_get_data(1));
              ploc = find_link_by_name(name1); /* Find the location stated */
              if (NULL != ploc){ /* If it found the location stated */
                int old_id = ploc->id;
                ploc->id = 1;
                set_start = line_number;

                /* Iterate through the entire world of locations */
                for (pll tmp_ptr = the_world; tmp_ptr; tmp_ptr = tmp_ptr->next){
                  ploc = tmp_ptr->data;
                  /* Iterate through each location's direction */
                  for(int dir=NORTH;dir<=DOWN;dir++){
                    if (ploc->exits[dir] == old_id)
                      ploc->exits[dir] = 1;
                    else if (ploc->exits[dir] == -old_id){
                      ploc->exits[dir] = -1;
                    }
                  }
                }
              }

            /* Allows only one location to be the start point for every player
             * once successfully set, subsequent CSV records of type zero
             * are ineffective
             */
            } else {
              fprintf(stderr,"Attempt to reassign start location at line %d "
                "of %s originally set in line %d\n",
                line_number, worldname, set_start);
            }
            break;

          case 1:
            /* Add location */
            strcpy(loc.name,csv_get_data(1));
            strcpy(loc.description,csv_get_data(2));
            loc.id = find_highest_id(); // Auto-increment the ID as needed
#if DEBUG == 1
            fprintf(stderr,"name:%s\ndescription:%s\n",loc.name,loc.description);
#endif
            add_location(loc);
            break;

          case 2:
            /* Create a link between two locations */
            strcpy(name1,csv_get_data(1));
            strcpy(name2,csv_get_data(2));
            strcpy(direction,csv_get_data(3));
            strcpy(exit_type,csv_get_data(4));
            link_type = !strcmp(exit_type,"entry")?1:!strcmp(exit_type,"exit")?2:3;

            link_by_names(name1,name2,get_direction_from_string(direction),link_type);
            break;
        }
        csv_done(); /* Clears CSV data from memory */
      }
      /* Comments are implicitly ignored */
    }
    fclose(csv); /* Clears file from memory */
  }
}

void armageddon(void){
  del_all_players();
  delete_all(&the_world,free);
}


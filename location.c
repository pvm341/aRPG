#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "location.h"

tlocation dummy[7] =  {
                        {1UL,{-1,-1, 3,-1,-1,-1},"This is the north dummy location", NULL},
                        {2UL,{-1,-1,-1, 3,-1,-1},"This is the east dummy location", NULL},
                        {3UL,{ 3,-1,-1,-1,-1,-1},"This is the south dummy location", NULL},
                        {4UL,{ 0, 1, 2, 4, 5, 6},"This is the central dummy location", NULL},
                        {5UL,{-1, 3,-1,-1,-1,-1},"This is the west dummy location", NULL},
                        {6UL,{-1,-1,-1,-1,-1, 3},"This is the up dummy location", NULL},
                        {7UL,{-1,-1,-1,-1, 3,-1},"This is the down dummy location", NULL},
                     };
char *directions[7] = {"north", "east", "south", "west","up", "down","Quit"};

/**
 * This dummy list display for proof of concept
 */

int get_new_loc(const int loc_id){
  char direction[12];
  int c, d, i, notfound=1;
  do {
    fprintf(stdout,"%s\n",dummy[loc_id].description);
    fprintf(stdout,"Exists are available in the directions\n");
    for(i=0, c=0; i<6; i++){
      if (dummy[loc_id].exits[i]>=0) {
        if (c++){
          fprintf(stdout,",");
        }
        fprintf(stdout,"%s",directions[i]);
      }
    }
    fprintf(stdout,"\nWhich direction ?");
    fgets(direction,11,stdin);
    for(d=0;d<11;d++)
      if (direction[d]=='\n')
        direction[d]=0;
    d=-1;
    while ((d<=6 && notfound)){
      d++;
      notfound = strcmp(direction,directions[d]);
      if (d<6){
        notfound |= !(dummy[loc_id].exits[d]>=0);
      }
    }
  } while (notfound);
  if (d == 6){
    d=-d;
  } else {
    d = (int)dummy[loc_id].exits[d];
  }
  return d;
}

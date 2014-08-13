#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "location.h"
#include "cmdpu.h"

tlocation dummy[7] =  {
                        {1UL,{-1,-1, 3,-1,-1,-1},"This is the north dummy location", NULL},
                        {2UL,{-1,-1,-1, 3,-1,-1},"This is the east dummy location", NULL},
                        {3UL,{ 3,-1,-1,-1,-1,-1},"This is the south dummy location", NULL},
                        {4UL,{ 0, 1, 2, 4, 5, 6},"This is the central dummy location", NULL},
                        {5UL,{-1, 3,-1,-1,-1,-1},"This is the west dummy location", NULL},
                        {6UL,{-1,-1,-1,-1,-1, 3},"This is the up dummy location", NULL},
                        {7UL,{-1,-1,-1,-1, 3,-1},"This is the down dummy location", NULL},
                     };

/**
 * This dummy list display for proof of concept
 */

int *display_location(const int loc_id){
  int c, i;

  fprintf(stdout,"%s\n",dummy[loc_id].description);
  fprintf(stdout,"Exits available are (");
  for(i=0, c=0; i<6; i++){
    if (dummy[loc_id].exits[i]>=0) {
      if (c++){
        fprintf(stdout,",");
      }
      fprintf(stdout,"%s",commands[i]);
    }
  }
  fprintf(stdout,")\nWhat to do ?");
  return dummy[loc_id].exits;
}

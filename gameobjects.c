#include <stdio.h>
#include <stdlib.h>

#include "gameobjects.h"

/*
 * Globals
 */
extern FILE *nul;

typedef enum {ARROW,
              CARBON,
              FEATHERS,
              GUN_POWDER,
              LONGBOW,
              PESTLE_MORTAR,
              QUIVER,
              SALTPETER, STRING, SULPHUR,
              TINDER_BOX, TORCH, TORCH_HOLDER,
              WOOD, WOOD_CUTTERS_AXE,
              EOI
             } GAME_ITEMS;


tobject game_items[EOI] =
  {
    {"arrow",             2,  1, 1,{WOOD,STRING,FEATHERS,EOI}},
    {"carbon",            1,  1, 1,{EOI,EOI,EOI,EOI}},
    {"feathers",          1,  1, 1,{EOI,EOI,EOI,EOI}},
    {"gun powder",       30, 16, 1,{CARBON,SALTPETER,SULPHUR,PESTLE_MORTAR}},
    {"longbow",         119, 48, 1,{WOOD, STRING, EOI,EOI}},
    {"pestle & mortar", 239, 16, 1,{EOI,EOI,EOI,EOI}},
    {"quiver",           30,  2,20,{EOI,EOI,EOI,EOI}},
    {"saltpeter",         1,  1, 1,{EOI,EOI,EOI,EOI}},
    {"string",            2,  1, 1,{EOI,EOI,EOI,EOI}},
    {"sulphur",           1,  1, 1,{EOI,EOI,EOI,EOI}},
    {"tinder box",      239,  8, 1,{EOI,EOI,EOI,EOI}},
    {"torch",            60, 12, 1,{EOI,EOI,EOI,EOI}},
    {"torch holder",      0, -1, 1,{EOI,EOI,EOI,EOI}},
    {"wood",              1,  1, 1,{EOI,EOI,EOI,EOI}},
    {"wood cutters axe",239, 64, 1,{EOI,EOI,EOI,EOI}}
  };

/*
 * Helper functions
 */

void spacer(char ch, int f, int a){
  for (int i = a; i<f;i++)
    fputc(ch,stdout);
}

int stuffs(char* game_item_string, int spacer_attrib, int w){
    w=fprintf(nul,"%s",game_item_string);
    fprintf(stdout,"%s,",game_item_string);
    spacer(' ',spacer_attrib,w);
    return w;
}

/* Alternative of stuffs, for integers */
int stuffd(int game_item_int, int spacer_attrib, int w){
    w=fprintf(nul,"%d",game_item_int);
    fprintf(stdout,"%d,",game_item_int);
    spacer(' ',spacer_attrib,w);
    return w;
}

/*
 * Primary Functions
 */

void display_items_list(){
  int idx,w;
  printf("ITEM                 VALUE  WEIGHT  REQUIREMENTS TO MAKE THIS\n");
  printf("----                 -----  ------  -------------------------\n");
  for (GAME_ITEMS item = ARROW; item<EOI; item++){
    w = stuffs(game_items[item].name, 20, w);
    w = stuffd(game_items[item].value, 6, w);
    w = stuffd(game_items[item].weight,6, w);

    if (EOI != game_items[item].requirements[0]){
      printf("  ");
      for(idx=0;
          MAX_REQUIREMENTS>idx &&
          EOI != game_items[item].requirements[idx];
          idx++){
        if (MAX_REQUIREMENTS-1 > idx && idx &&
            EOI != game_items[item].requirements[idx+1]){
          printf(", ");
        } else if (MAX_REQUIREMENTS > idx && idx) {
          printf(" and ");
        }
        printf("%s",game_items[game_items[item].requirements[idx]].name);
      }
//      printf("]");
    }
//    if (0==idx){
//      printf("Nothing");
//    } else if (3>idx){
//      printf("Nothing else");
//    }
    printf("\n");
  }
}

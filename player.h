#ifndef PLAYER_INCLUDED
#define PLAYER_INCLUDED

#include "list.h"
#include "inventory.h"

struct splayer {
char name[26];
int no_items_in_inventory;
pll inventory;
};

typedef struct splayer tplayer, *pplayer, **dplayer;

void add_player(char *name);
//void add_item_to_inv_of(char *name, pinventoryitem inv_item);
void load_player(char *name);
void save_player(char *name);
void del_player(char *name);
void del_all_players();
pll find_player(char *name);
void display_all_players();

//  tplayer player[2] =             {
//                                    { "Bert", {
//                                                {"Bronze Pickaxe",42},
//                                                {"Tinderbox",119},
//                                                {"Cooked Chicken",239},
//                                                {"Raw Chicken",179},
//                                                {"Cooking Pot",719},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0}
//                                              }
//                                    },
//                                    { "Paul",   {
//                                                ,
//                                                {"Tinderbox",119},
//                                                {"Cooked Beef Steak",239},
//                                                {"Raw Beef Steak",179},
//                                                {"Cooking Pot",719},
//                                                {"Gold Ring",24000-1},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0},
//                                                {"",0}
//                                              }
//
//                                  }
//                                  };
 // display_player(player[0]);
 // display_player(player[1]);

#endif // PLAYER_INCLUDED

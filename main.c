/*
 * File:   main.c
 * Author: paul
 *
 * Created on 10 August 2014, 06:30
 */

/* referenced pages
 * https://en.wikipedia.org/wiki/ANSI_escape_code
 */

#include <stdio.h>
#include <stdlib.h>
#include "gameobjects.h"
#include "inventory.h"
#include "player.h"
#include "location.h"
#include "cmdpu.h"

/*
 *
 */
FILE *nul;
int main(int argc, char** argv) {
  if (NULL == (nul  = fopen("/dev/null","w"))){
    if (NULL == (nul = fopen("nul:","w"))){
      fprintf(stderr,"Unable to open nul\nExiting now\n");
      return -1;
    }
  };
  genesis();
  tinventoryitem the_inv_item;
  the_inv_item = create_inv_item("Iron Axe",83);
  add_player("Paul");
  display_all_players();
  add_player("Bert");
  add_item_to_inventory("Bert", the_inv_item);
  display_all_players();
  the_inv_item = create_inv_item("Raw Chicken",23);
  add_item_to_inventory("Bert", the_inv_item);
  display_all_players();
  the_inv_item = create_inv_item("Bronze Axe",30);
  add_item_to_inventory("Paul",the_inv_item);
  display_all_players();
  the_inv_item = create_inv_item("Minted Lamb Steak",35);
  add_item_to_inventory("Paul",the_inv_item);
  display_all_players();
  the_inv_item = create_inv_item("Gold ring",252*100);
  add_item_to_inventory("Paul",the_inv_item);
  the_inv_item = create_inv_item("Tinder Box",239);
  add_item_to_inventory("Paul",the_inv_item);
  display_all_players();
  del_item_from_inventory("Paul","Minted Lamb Steak");
  del_player("Fred");
  display_all_players();
  add_player("Fred");
  display_all_players();
#ifdef GAMEOBJECTS_H
  display_items_list();
#endif
  game_loop();
  armageddon();
  fclose(nul);
  return (EXIT_SUCCESS);
}


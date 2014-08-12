/*
 * File:   main.c
 * Author: paul
 *
 * Created on 10 August 2014, 06:30
 */

#include <stdio.h>
#include <stdlib.h>
#include "inventory.h"
#include "player.h"
#include "location.h"

/*
 *
 */
FILE *nul;
int main(int argc, char** argv) {
  int loc_id;
  nul  = fopen("/dev/null","w");
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
  the_inv_item = create_inv_item("Tinder Box",239);
  add_item_to_inventory("Paul",the_inv_item);
  display_all_players();
  del_item_from_inventory("Paul","Minted Lamb Steak");
  del_player("Fred");
  display_all_players();
  add_player("Fred");
  display_all_players();
  del_all_players();
  display_all_players();
  loc_id = 3;
  do {
    loc_id = get_new_loc(loc_id);
  } while(loc_id>=0);
  fclose(nul);
  return (EXIT_SUCCESS);
}


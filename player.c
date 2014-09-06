#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "currencylsd.h"
#include "inventory.h"
#include "player.h"
#include "list.h"

/* Helper functions */
static pll head_of_player_list=NULL;

static int cmp_players_alpha(const pplayer player1, const pplayer player2){
  return strcmp(player1->name,player2->name);
}

static int cmp_player_name(pplayer player, char *name){
  return strcmp(player->name,name);
}

static int display_player(FILE *file, pplayer player){
  int total = 0;
  char pricestr[21];
  fprintf(file,"%s has %d %s in their inventory list\n",
      player->name,player->no_items_in_inventory,
      player->no_items_in_inventory==1?"item":"items");
  total = display_inventory_list(file,player->inventory);
  printf("This makes %s worth %s\n\n",player->name,price_disp(pricestr,total));
  return total;
}

static void del_current_player(pll ptr){
  pplayer player = ptr->data;
  if (player->inventory){
      delete_all(&player->inventory,free);
  }
}

/* Core functionality */
void add_player(char *name){
  if (NULL == find_item_in_list((pll)head_of_player_list,name,cmp_player_name)){
    pplayer new_player;
    new_player = (pplayer) malloc(sizeof(tplayer));
    if ( NULL != new_player){
      strcpy(new_player->name,name);
      new_player->no_items_in_inventory=0;
      new_player->inventory = NULL;
      add_node_to_ordered_list(&head_of_player_list, new_player,
            cmp_players_alpha);
    }
  }
  // TODO: Need error reporting code
}

void load_player(char *name){
}

void save_player(char *name){
}

void del_player(char *name){
  int okay = del_list_item(&head_of_player_list,name,del_current_player,
    cmp_player_name);
  if (!okay)
    printf("Player %s not found!\n",name);
}

void display_all_players(){
  display_list(stdout,head_of_player_list,display_player);
}

void del_all_players(){
  delete_all(&head_of_player_list,del_current_player);
}

//void add_item_to_inv_of(char *name, pinventoryitem inv_item){
//  pll ptr = NULL;
//  ptr = find_item(head_of_player_list,name,cmp_player_name);
//  if ( ptr != NULL){
//    pplayer player=ptr->data;
//    add_list_node(&(player->inventory),inv_item);
//  }
//}

pll find_player(const char *name){
  return find_item_in_list(head_of_player_list,name,cmp_player_name);
}

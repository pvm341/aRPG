#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "inventory.h"
#include "currencylsd.h" // price_disp()
#include "player.h"
extern FILE *nul;


/*
 * Helper functions
 */ 
static int cmp_descending_value(const pinventoryitem item2, const pinventoryitem item1)
{
  return item1->value - item2->value;
}

static int cmp_inv_item_to_item_name(const pinventoryitem item, const char *item_name){
  return strcmp(item->name,item_name);
}

static void find_widths_inventory_item(int *widths, pinventoryitem inv_item){
  char pricestr[21];
  int w;
  w= fprintf(nul,"%s",inv_item->name);
  if (w > widths[0])
    widths[0] = w;
  price_disp(pricestr,inv_item->value);
  w = fprintf(nul,"%s",pricestr);
  if (w > widths[1])
    widths[1] = w;
}

static int display_inventory_item(FILE *file, pinventoryitem inv_item, int *widths, int c){
  char pricestr[21];
  int i,w;
  price_disp(pricestr,inv_item->value);
  fprintf(file,"|%2d",c);
  w=fprintf(file,"|%s",inv_item->name);
  i=widths[0]-w;
  while (i-- >-1 ){
    fputc(' ',file);
  }
  w = fprintf(nul,"%s",pricestr);
  i = widths[1] - w;
  fputc('|',file);
  while (i--  ){
    fputc(' ',file);
  }
  fprintf(file," %s|\n",pricestr);
  return inv_item->value;
}

/*
 * Functionality
 */
int display_inventory_list(FILE *file, pll inventory_list){
  int widths[2]={-1,-1};
  int total = 0;
  pll list_ptr;
  pinventoryitem inv_item;
  list_ptr = inventory_list;
  while(list_ptr != NULL) {
    inv_item = list_ptr->data;
    find_widths_inventory_item(widths,inv_item);
    list_ptr = list_ptr->next;
  }
  int c=0;
  fprintf(file,"+--+");
  for (int i = 0;i<widths[0]; i++){
    fprintf(file,"-");
  }
  fprintf(file,"+");
  for (int i = 0;i<widths[1]; i++){
    fprintf(file,"-");
  }
  fprintf(file,"+\n");
  for ( list_ptr = inventory_list; list_ptr; list_ptr = list_ptr->next){
    inv_item = list_ptr->data;
    total += display_inventory_item(file,inv_item,widths,++c);
  }
  fprintf(file,"+--+");
  for (int i = 0;i<widths[0]; i++){
    fprintf(file,"-");
  }
  fprintf(file,"+");
  for (int i = 0;i<widths[1]; i++){
    fprintf(file,"-");
  }
  fprintf(file,"+\n");
  return total;
}

tinventoryitem create_inv_item(const char *name, const int value){
  tinventoryitem an_inv_item;
  strcpy(an_inv_item.name,name);
  an_inv_item.value = value;
  return an_inv_item;
}

int add_item_to_inventory(const char *name, const tinventoryitem an_inv_item){
    int error = 1; //assume error will result
    pll ptr_player = find_player(name);
    pplayer player = ptr_player->data;
    pinventoryitem new_inv_item =(pinventoryitem) malloc(sizeof(tinventoryitem));
    if (ptr_player != NULL && new_inv_item != NULL){
        memcpy(new_inv_item,&an_inv_item,sizeof(tinventoryitem));
        add_ordered_list_node(&player->inventory,new_inv_item,cmp_descending_value);
        player->no_items_in_inventory++;
        error = 0;
    }
    return error;
}

int del_item_from_inventory(const char *name, const char *inv_item){
  int okay = 0; //assume error will result
  pll ptr_player = find_player(name);
  pplayer player = ptr_player->data;
  if ((okay = del_list_item(&player->inventory,inv_item,free,cmp_inv_item_to_item_name))){
    player->no_items_in_inventory--;
  }
  return !okay;
}

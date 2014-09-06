#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "mob.h"

static pll head_of_mob_list=NULL;

/* Helper functions */

/*
 * This compares the names of two different mobs
 *
 * return <0 if mob1->name is less than mob2->name
           0 if they contain the same characters
          >0 if mob1->name is greater than mob2->name
 */
static int cmp_mobs_alpha(const pmob mob1, const pmob mob2)
{
  return strcmp(mob1->name,mob2->name);
}

/*
 * This compares the name of a mob and a string to ensure
 * looking for mob with that name. for unique name.
 *
 * return <0 if mob1->name is less than mob2->name
           0 if they contain the same characters
          >0 if mob1->name is greater than mob2->name
 */
static int cmp_mobs_name(const pmob mob, const char *name)
{
  return strcmp(mob->name,name);
}

/* CRUD functionality */
void create_mob(char *name){
  if(NULL == find_item_in_list((pll)head_of_mob_list, name, cmp_mobs_name)){
    pmob new_mob;
    new_mob = (pmob) malloc(sizeof(tmob));
    if( NULL != new_mob){
      strcpy(new_mob->name,name);
      new_mob->no_items_in_inventory=0;
      new_mob->inventory = NULL;
      add_node_to_ordered_list(&head_of_mob_list, new_mob, cmp_mobs_alpha);
    }
  }
}


pmob retrieve_mob(char *name){
  pll mob_list_entry = find_item_in_list(head_of_mob_list,name,cmp_mobs_name);
  return mob_list_entry?mob_list_entry->data:NULL;
}

void update_mob(void){
}

void delete_mob(void){
}


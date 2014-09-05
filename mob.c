#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "mob.h"

static pll head_of_mob_list=NULL;

/* Helper functions */
static int cmp_mobs_alpha(const pmob mob1, const pmob mob2)
{
    return 0;
}

static int cmp_mobs_name(pmob mob, char *name)
{
    return 0;
}

/* CRUD functionality */
void create_mob(char *name){
    if(NULL == find_item((pll)head_of_mob_list, name, cmp_mobs_name)){
        pmob new_mob;
        new_mob = (pmob) malloc(sizeof(tmob));
        if( NULL != new_mob){
            strcpy(new_mob->name,name);
            new_mob->no_items_in_inventory=0;
            new_mob->inventory = NULL;
            add_ordered_list_node(&head_of_mob_list, new_mob, cmp_mobs_alpha);
        }
    }
}


void retrieve_mob(void){
}

void update_mob(void){
}

void delete_mob(void){
}


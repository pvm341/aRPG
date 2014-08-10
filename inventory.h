#ifndef INVENTORY_H_INCLUDED
#define INVENTORY_H_INCLUDED
#include "list.h"
struct sinventoryitem {
  char name[40];
  int  value;
};

typedef struct sinventoryitem tinventoryitem, *pinventoryitem, **dinventoryitem;

tinventoryitem create_inv_item(const char *name, const int value);
int display_inventory_list(FILE *file, pll inventory_list);
int add_item_to_inventory(const char *name, const tinventoryitem an_inv_item);

#endif // INVENTORY_H_INCLUDED

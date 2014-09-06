
#include "list.h"
#include "inventory.h"

struct smob {
  char name[26];
  int no_items_in_inventory;
  pll inventory;
};


typedef struct smob tmob, *pmob, **dmob;
void create_mob(char *name);
pmob retrieve_mob(char *name);
void update_mob(void);
void delete_mob(void);

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#define MAX_REQUIREMENTS 4
// gunpowder requires Carbon, Sulphur, and Potassium Nitrate.
// object   - Resource        - Purchased from
// Carbon   - Coal/charcol    - an apothecary
// Sulphur  - volcanic area   - an apothecary
// KNO3     -                 - an apogathery

struct sobject {
char *name;
int value;
int weight; // -ve if fixed object +ve if player can pick it up
int maxqty; // if not one then a container.
unsigned int requirements[MAX_REQUIREMENTS];
};

typedef struct sobject tobject, *pobject, **dobject;
void display_items_list();
#endif

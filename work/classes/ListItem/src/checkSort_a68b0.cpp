#include "class.h"

struct Status { Ship *getShip(); };
extern Status *status;
extern "C" int Item_getSort_li(Item *it);              // blx 0x71908
extern "C" int ListItem_sortCmp(Ship *ship, int sort); // tail b.w 0x1abe28

// ListItem::checkSort() — field 0x10 is reloaded (not cached) before getSort.
extern "C" int ListItem_checkSort(ListItem *self) {
    if (self->f_10 == 0)
        return 0;
    Ship *ship = status->getShip();
    int sort = Item_getSort_li(self->f_10);
    return ListItem_sortCmp(ship, sort);
}

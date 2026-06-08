#include "class.h"

struct Status { Ship *getShip(); };
extern Status *status;
extern "C" int Item_getType_li(Item *it);                  // blx 0x718fc
extern "C" int Ship_getFreeSlots_li(Ship *ship, int type); // blx 0x733e4

// ListItem::checkSlot()
extern "C" int ListItem_checkSlot(ListItem *self) {
    int r = 0;
    if (self->f_10 != 0) {
        Ship *ship = status->getShip();
        int type = Item_getType_li(self->f_10);
        if (Ship_getFreeSlots_li(ship, type) > 0)
            r = 1;
    }
    return r;
}

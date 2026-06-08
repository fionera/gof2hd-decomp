#include "class.h"

struct Status { int getCredits(); };
extern Status *status;
extern "C" int Ship_getPrice_li(Ship *s);          // blx 0x71b00
extern "C" int Item_getSinglePrice_li(Item *it);    // blx 0x71944

// ListItem::checkCredits()
extern "C" bool ListItem_checkCredits(ListItem *self) {
    int price;
    Ship *sh = self->f_c;
    if (sh == 0) {
        Item *it = self->f_10;
        if (it == 0)
            return false;
        price = Item_getSinglePrice_li(it);
    } else {
        price = Ship_getPrice_li(sh);
    }
    int credits = status->getCredits();
    return price <= credits;
}

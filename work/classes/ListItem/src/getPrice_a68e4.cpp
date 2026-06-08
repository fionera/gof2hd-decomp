#include "class.h"

extern "C" int Ship_getPrice_li(Ship *s);     // 0x1abe38 tail veneer
extern "C" int Item_getSinglePrice_li();       // 0x1abe48 tail veneer

// ListItem::getPrice()
extern "C" int ListItem_getPrice(ListItem *self) {
    Ship *sh = F<Ship *>(self, 0xc);
    if (sh)
        return Ship_getPrice_li(sh);
    if (F<int>(self, 0x10) == 0)
        return 0;
    return Item_getSinglePrice_li();
}

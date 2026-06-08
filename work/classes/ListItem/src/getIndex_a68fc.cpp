#include "class.h"

extern "C" int Ship_getIndex_li(Ship *s);    // tail b.w 0x1abe58
extern "C" int Item_getIndex_li(Item *it);    // tail b.w 0x1abe68
extern "C" int BluePrint_getIndex_li(BluePrint *bp);  // tail b.w 0x1abe78

// ListItem::getIndex() — default 999999 (literal pool constant) when nothing set;
// the PendingProduct path reads its index at +0x14, selected via ite eq.
extern "C" int ListItem_getIndex(ListItem *self) {
    Ship *sh = F<Ship *>(self, 0xc);
    if (sh)
        return Ship_getIndex_li(sh);
    Item *it = F<Item *>(self, 0x10);
    if (it)
        return Item_getIndex_li(it);
    BluePrint *bp = F<BluePrint *>(self, 0x8);
    if (bp)
        return BluePrint_getIndex_li(bp);
    void *pp = F<void *>(self, 0x18);
    if (pp == 0)
        return 999999;
    return *(int *)((char *)pp + 0x14);
}

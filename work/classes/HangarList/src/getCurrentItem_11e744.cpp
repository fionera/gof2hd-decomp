#include "class.h"

extern "C" ListItem *HangarList_getCurrentItem_tail(HangarList *self,
                                                    ListItem *item);

ListItem *HangarList::getCurrentItem() {
    return HangarList_getCurrentItem_tail(this, F<ListItem *>(this, 0x8));
}

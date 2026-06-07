#include "class.h"

Array<ListItem *> *HangarList::getCurrentTabItems() {
    Array<Array<ListItem *> *> *tabs = F<Array<Array<ListItem *> *> *>(this, 0x0);
    uint32_t current = F<uint32_t>(this, 0x4);
    return tabs->data[current];
}

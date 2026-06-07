#include "class.h"

uint32_t HangarList::getCurrentLength() {
    Array<Array<ListItem *> *> *tabs = F<Array<Array<ListItem *> *> *>(this, 0x0);
    uint32_t current = F<uint32_t>(this, 0x4);
    Array<ListItem *> *items = tabs->data[current];
    return items != 0 ? items->length : 0;
}

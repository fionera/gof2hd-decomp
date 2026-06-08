#include "class.h"

uint32_t HangarList::getCurrentLength() {
    Array<Array<ListItem *> *> *tabs = F<Array<Array<ListItem *> *> *>(this, 0x0);
    uint32_t current = this->f_4;
    Array<ListItem *> *items = tabs->data[current];
    return items != 0 ? items->length : 0;
}

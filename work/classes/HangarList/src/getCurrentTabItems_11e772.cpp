#include "class.h"

Array<ListItem *> *HangarList::getCurrentTabItems() {
    Array<Array<ListItem *> *> *tabs = F<Array<Array<ListItem *> *> *>(this, 0x0);
    uint32_t current = this->f_4;
    return tabs->data[current];
}

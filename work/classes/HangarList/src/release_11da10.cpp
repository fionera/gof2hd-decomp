#include "class.h"

void HangarList::release() {
    Array<Array<ListItem *> *> *tabs =
        F<Array<Array<ListItem *> *> *>(this, 0x0);
    if (tabs != 0) {
        uint32_t off = 0;
        uint32_t i = 0;
        while (i < tabs->length) {
            Array<ListItem *> **data = tabs->data;
            Array<ListItem *> *items =
                *(Array<ListItem *> **)((char *)data + off);
            Array<ListItem *> **slot;
            if (items == 0) {
                slot = data + i;
            } else {
                if (items->length != 0) {
                    ArrayReleaseClasses_ListItem(items);
                    tabs = F<Array<Array<ListItem *> *> *>(this, 0x0);
                    data = tabs->data;
                    items = *(Array<ListItem *> **)((char *)data + off);
                    if (items == 0) {
                        slot = data + i;
                        goto clear_slot;
                    }
                }
                operator_delete(Array_ListItem_dtor(items));
                tabs = F<Array<Array<ListItem *> *> *>(this, 0x0);
                slot = (Array<ListItem *> **)((char *)tabs->data + off);
            }
        clear_slot:
            *slot = 0;
            uint32_t next = i + 1;
            off += 4;
            tabs = F<Array<Array<ListItem *> *> *>(this, 0x0);
            i = next;
        }
        ArrayReleaseClasses_ArrayListItem(tabs);
        tabs = F<Array<Array<ListItem *> *> *>(this, 0x0);
        if (tabs != 0) {
            operator_delete(Array_ArrayListItem_dtor(tabs));
        }
    }
    this->f_0 = 0;
}

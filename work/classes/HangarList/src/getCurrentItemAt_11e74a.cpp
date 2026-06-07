#include "class.h"

ListItem *HangarList::getCurrentItemAt(int index) {
    Array<Array<ListItem *> *> *tabs = F<Array<Array<ListItem *> *> *>(this, 0x0);
    if (tabs != 0) {
        uint32_t current = ((volatile uint32_t *)this)[1];
        uint32_t dataWord = ((volatile uint32_t *)tabs)[1];
        Array<ListItem *> **data = (Array<ListItem *> **)dataWord;
        uint32_t itemsWord = ((volatile uint32_t *)data)[current];
        Array<ListItem *> *items = (Array<ListItem *> *)itemsWord;
        if (items != 0) {
            ListItem *result = 0;
            if (-1 < index) {
                int length = items->length;
                if (length > index) {
                    result = items->data[index];
                }
                return result;
            }
            return 0;
        }
    }
    return 0;
}

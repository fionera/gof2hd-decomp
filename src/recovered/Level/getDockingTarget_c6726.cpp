#include "class.h"

int Level::getDockingTarget(int index) {
    Array *list = (Array *)enemies;
    if (list != 0) {
        int found = 0;
        for (unsigned int i = 0; i < list->size; i = i + 1) {
            int obj = ((int *)list->data)[i];
            if (*(char *)(obj + 0x70) != 0) {
                if (found == index) {
                    return obj;
                }
                found = found + 1;
            }
        }
    }
    return 0;
}

#include "class.h"

int Level::getNumDockingTargets() {
    Array *list = (Array *)enemies;
    if (list != 0) {
        int total = 0;
        for (unsigned int i = 0; list->size != i; i = i + 1) {
            total = total + *(unsigned char *)(((int *)list->data)[i] + 0x70);
        }
        return total;
    }
    return 0;
}

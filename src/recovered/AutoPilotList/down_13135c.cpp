#include "class.h"

// AutoPilotList::down() - advance selection to the next non-empty entry, wrapping at 4.
extern "C" void _ZN13AutoPilotList4downEv(AutoPilotList *self) {
    void **data = F<Array<void *> *>(self, 0x10)->data;
    int i = F<int>(self, 0x0);
    int n;
    do {
        n = 0;
        if (i < 4)
            n = i + 1;
        i = n;
    } while (data[i] == 0);
    F<int>(self, 0x0) = i;
}

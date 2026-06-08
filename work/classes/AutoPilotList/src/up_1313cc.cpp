#include "class.h"

// AutoPilotList::up() - move selection to the previous non-empty entry, wrapping at 0.
extern "C" void _ZN13AutoPilotList2upEv(AutoPilotList *self) {
    void **data = F<Array<void *> *>(self, 0x10)->data;
    int i = F<int>(self, 0x0);
    int n;
    do {
        n = 4;
        if (0 < i)
            n = i - 1;
        i = n;
    } while (data[i] == 0);
    F<int>(self, 0x0) = i;
}

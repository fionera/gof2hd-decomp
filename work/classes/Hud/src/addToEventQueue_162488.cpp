#include "class.h"

extern "C" void Hud_addToEventQueue(Hud *self, ListItem *item)
{
    unsigned int idx = 0;
    unsigned int data;
    int off;
    do {
        unsigned int next = idx + 1;
        unsigned int *arr = (unsigned int *)self->f_264;
        if (next >= arr[0])
            return;
        data = arr[1];
        off = idx * 4;
        idx = next;
    } while (I((void *)data, off + 4) != 0);
    P((void *)data, idx * 4) = item;
    UC(self, 0x26c) = 1;
}

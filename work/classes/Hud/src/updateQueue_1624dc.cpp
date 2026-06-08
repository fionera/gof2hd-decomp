#include "class.h"

extern "C" void *ListItem_dtor(void *p);   // ListItem::~ListItem
extern "C" void operator_delete(void *p);

extern "C" void Hud_updateQueue(Hud *self, int dt)
{
    int t = I(self, 0x268) + dt;
    I(self, 0x268) = t;
    int v;
    if (t >= 0xfa1) {
        I(self, 0x268) = 0;
        int *p = (int *)I(self->f_264, 4);
        if ((void *)p[0] != 0) {
            void *q = ListItem_dtor((void *)p[0]);
            operator_delete(q);
            p = (int *)I(self->f_264, 4);
        }
        p[0] = 0;
        unsigned int i = 0;
        unsigned int d;
        while (true) {
            d = ((unsigned int *)self->f_264)[1];
            if (*(unsigned int *)self->f_264 <= i + 1)
                break;
            *(int *)(d + i * 4) = *(int *)(d + i * 4 + 4);
            i = i + 1;
        }
        if (*(int *)(d + 4) == 0) {
            UC(self, 0x26c) = 0;
        }
        v = 0;
    } else {
        if (t < 0x7d1)
            return;
        if (I(self, 0x270) != 0)
            return;
        v = 1;
    }
    I(self, 0x270) = v;
}

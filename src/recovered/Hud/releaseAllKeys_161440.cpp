#include "class.h"

extern "C" void Hud_releaseAllKeys(Hud *self)
{
    I(self, 0x284) = 0;
    for (int i = 0; i != 0x19; i++) {
        int *p = *(int *volatile *)((char *)self + 0x28c);
        if (p != 0) {
            int *a = (int *)p[1];
            if (a[i] != 0)
                a[i] = 0;
        }
        int *q = *(int *volatile *)((char *)self + 0x290);
        q[i] = 0;
    }
    I(self, 0x288) = 0;
}

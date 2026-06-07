#include "class.h"

extern "C" void Achievements_setMedals(Achievements *self, int *src, int count)
{
    int i = 0;
    for (i = 0; i < count; i = i + 1) {
        unsigned v = (unsigned)src[i];
        unsigned *medals = (unsigned *)F<int *>(self, 0x0);
        if (v < 4)
            medals[i] = v;
        else
            medals[i] = 0;
    }
    for (; count < 0x2d; count = count + 1) {
        F<int *>(self, 0x0)[count] = 0;
    }
}

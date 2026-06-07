#include "class.h"

extern "C" int Achievements_init(Achievements *self)
{
    int *medals = F<int *>(self, 0x0);
    int i;
    for (i = 0; i != 0x2d; i = i + 1)
        medals[i] = 0;
    *medals = 1;
    int *newMedals = F<int *>(self, 0x4);
    int j;
    for (j = 0; j != 0x2d; j = j + 1)
        newMedals[j] = 0;
    F<uint8_t>(self, 0x18) = 0;
    F<int>(self, 0x1c) = 0;
    F<int>(self, 0x8) = 0;
    F<int>(self, 0xc) = 0;
    F<int>(self, 0x10) = 0;
    F<int>(self, 0x14) = 0;
    return (int)((char *)self + 0x8);
}

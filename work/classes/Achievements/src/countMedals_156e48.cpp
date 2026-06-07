#include "class.h"

extern "C" void Achievements_countMedals(Achievements *self)
{
    int *medals = F<int *>(self, 0x0);
    int total = 0;
    int golds = 0;
    F<int>(self, 0x24) = 0;
    int i;
    for (i = 0; i != 0x24; i = i + 1) {
        int v = medals[i];
        if (v != 0) {
            total = total + 1;
            if (v == 1) {
                F<int>(self, 0x24) = total;
                golds = golds + 1;
            } else {
                F<int>(self, 0x24) = total;
            }
        }
    }
    int j;
    int supers = 0;
    for (j = 0; j != 9; j = j + 1) {
        if (medals[0x24 + j] != 0)
            supers = supers + 1;
    }
    uint8_t allGold = (golds == 0x24);
    F<uint8_t>(self, 0x22) = (supers == 9) & allGold;
    F<uint8_t>(self, 0x21) = allGold;
    F<uint8_t>(self, 0x20) = (total == 0x24);
}

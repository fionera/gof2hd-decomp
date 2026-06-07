#include "class.h"
extern int **gShipDataRoot __attribute__((visibility("hidden")));
extern "C" void priceDecline(Ship *self) {
    int *q = *gShipDataRoot;
    int *table = *(int **)((char *)q + 4);
    int *entry = (int *)table[self->index];
    self->price = (int)((float)entry[0x14 / 4] / 1.25f);
}

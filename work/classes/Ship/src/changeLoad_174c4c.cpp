#include "class.h"
extern int **gWorldSingleton __attribute__((visibility("hidden")));
extern "C" void changeLoad(Ship *self, int delta) {
    int **p = gWorldSingleton;
    int cur = self->currentLoad;
    int newLoad = delta + cur;
    self->currentLoad = newLoad;
    int v = (self->baseLoad - newLoad) + self->cargoPlus;
    int *w = *p;
    if (w[0xdc / 4] < v) {
        w[0xdc / 4] = v;
    }
}

#include "class.h"
extern "C" int Item_getAmount(Item *it);
extern "C" void refreshValue(Ship *self);
extern int **gWorldSingleton __attribute__((visibility("hidden")));
extern "C" void setCargo(Ship *self, Array<Item*> *cargo) {
    self->currentLoad = 0;
    self->cargo = cargo;
    if (cargo != 0) {
        for (unsigned int i = 0; i < cargo->size; i = i + 1) {
            int amt = Item_getAmount(cargo->data[i]);
            cargo = self->cargo;
            self->currentLoad = amt + self->currentLoad;
        }
    }
    refreshValue(self);
    int v = (self->baseLoad + self->cargoPlus) - self->currentLoad;
    int *w = *gWorldSingleton;
    if (w[0xdc / 4] < v) {
        w[0xdc / 4] = v;
    }
}

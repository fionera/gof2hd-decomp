#include "class.h"
extern "C" void Array_Item_ctor(Array<Item*> *a);
extern "C" int *operatorNewArrayInt(unsigned int n);
extern "C" void refreshValue(Ship *self);

extern "C" Ship *_ZN4ShipC2Eiiiiiiiif(Ship *self, int index, int baseHP, int baseLoad,
                                       int value, int s0, int s1, int s2, int s3, float handling) {
    self->index = index;
    self->baseHP = baseHP;
    self->value = value;
    self->baseLoad = baseLoad;
    self->currentLoad = 0;
    self->price = value;
    int *slots = operatorNewArrayInt(0x10);
    self->slots = slots;
    slots[0] = s0;
    slots[1] = s1;
    slots[2] = s2;
    slots[3] = s3;
    self->handling = handling / 1.6f;
    Array<Item*> *eq = (Array<Item*> *)operatorNewArrayInt(0xc);
    Array_Item_ctor(eq);
    self->equipment = eq;
    ArraySetLength<Item*>(s1 + s0 + s2 + s3, *eq);
    self->cargo = 0;
    self->race = 0;
    self->mods = 0;
    self->numAddedDeviceSlots = 0;
    refreshValue(self);
    return self;
}

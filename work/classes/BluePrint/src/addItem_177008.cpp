#include "class.h"

extern "C" Array<int> *BluePrint_getIngredientList(BluePrint *self);
extern "C" void Item_setBlueprintAmount(Item *it, int amount);
extern "C" int Item_getIndex(Item *it);
extern "C" int Item_getSinglePrice(Item *it);
extern "C" void *Status_getStation(void);
extern "C" int Station_getIndex(void *station);
extern "C" void Station_getName(void *out, void *station);
extern "C" void *Galaxy_getStation(void *galaxy, int idx);
extern "C" void String_assign(void *dst, void *src);   // AbyssEngine::String::operator=
extern "C" void String_dtor(void *p);
extern "C" void *Station_dtor(void *p);
extern "C" void operator_delete(void *p);
// Hidden PC-relative pointer-to-the-global-Galaxy pointer.
extern void *const gGalaxyPtr __attribute__((visibility("hidden")));

// BluePrint::addItem(Item *item, int amount, int station)
extern "C" void BluePrint_addItem(BluePrint *self, Item *item, int amount, int station)
{
    if (amount != 0) {
        Item_setBlueprintAmount(item, 0);
        Array<int> *il = BluePrint_getIngredientList(self);
        if (il != 0) {
            for (uint32_t i = 0; i < il->length; i++) {
                if (il->data[i] == Item_getIndex(item)) {
                    F<Array<int> *>(self, 0x0)->data[i] -= amount;
                    F<int32_t>(self, 0x4) += Item_getSinglePrice(item) * amount;
                    if (station >= 0 && F<int32_t>(self, 0x10) < 0) {
                        F<int32_t>(self, 0x10) = station;
                        if (Station_getIndex(Status_getStation()) == station) {
                            char tmp[12];
                            Station_getName(tmp, Status_getStation());
                            String_assign((char *)self + 0x14, tmp);
                            String_dtor(tmp);
                        } else {
                            void *st = Galaxy_getStation(*(void **)gGalaxyPtr, station);
                            char tmp[12];
                            Station_getName(tmp, st);
                            String_assign((char *)self + 0x14, tmp);
                            String_dtor(tmp);
                            if (st != 0)
                                operator_delete(Station_dtor(st));
                        }
                    }
                    break;
                }
            }
        }
    }
}

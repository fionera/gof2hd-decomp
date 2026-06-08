#include "class.h"

extern "C" void *HangarList_getItems(void *list);
extern "C" int ListItem_isBluePrint(void *item);
extern "C" void *Ship_getCargo(void *ship);
extern "C" void *BluePrint_getIngredientList(void *bp);
extern "C" int Item_getIndex(void *item);
// Status singleton (pc-rel -> holder; *holder is the Status object).
__attribute__((visibility("hidden"))) extern void **g_hw_status2;
extern "C" void *Status_getShip(void *status);

extern "C" void HangarWindow_refreshCargoAvailabilityForBlueprints(HangarWindow *self)
{
    void *items = HangarList_getItems(F<void *>(self, 0x14));
    Array<void *> *arr = *(Array<void *> **)((char *)G<void *>(items, 0x4) + 0x8);
    if (arr == 0) return;
    for (uint32_t i = 0; i < arr->length; i++) {
        void *it = arr->data[i];
        F<uint8_t>((HangarWindow *)it, 0x45) = 0;
        if (it != 0 && ListItem_isBluePrint(it) != 0) {
            void *bp = G<void *>(it, 0x8);
            void *cargo = Ship_getCargo(Status_getShip(*g_hw_status2));
            Array<int> *ingr = (Array<int> *)BluePrint_getIngredientList(bp);
            if (cargo != 0) {
                void *base = G<void *>(bp, 0x0);
                for (uint32_t j = 0; j < ingr->length; j++) {
                    int *amts = *(int **)((char *)base + 0x4);
                    if (amts[j] > 0) {
                        Array<void *> *carr = (Array<void *> *)cargo;
                        for (uint32_t k = 0; k < carr->length; k++) {
                            if (Item_getIndex(carr->data[k]) == ingr->data[j]) {
                                F<uint8_t>((HangarWindow *)it, 0x45) = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

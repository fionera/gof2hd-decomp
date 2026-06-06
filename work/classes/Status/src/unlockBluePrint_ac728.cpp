#include "class.h"

extern "C" int BluePrint_getIndex(BluePrint *);
extern "C" void BluePrint_unlock(BluePrint *);

// Unlocks every blueprint whose item index matches `index`.
void Status::unlockBluePrint(int index) {
    Array<BluePrint *> *bps = (Array<BluePrint *> *)bluePrints;
    for (unsigned i = 0; i < bps->size; i = i + 1) {
        BluePrint *bp = bps->data[i];
        if (BluePrint_getIndex(bp) == index) {
            BluePrint_unlock(((Array<BluePrint *> *)bluePrints)->data[i]);
        }
    }
}

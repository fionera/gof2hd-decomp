#include "class.h"

extern "C" int BluePrint_getIndex(BluePrint *);
extern "C" int BluePrint_isUnlocked(BluePrint *);

// Returns whether the blueprint with item index `index` has been unlocked.
int Status::isBlueprintUnlocked(int index) {
    Array<BluePrint *> *bps = (Array<BluePrint *> *)bluePrints;
    unsigned i = 0;
    while (true) {
        if (bps->size <= i) {
            return 0;
        }
        if (BluePrint_getIndex(bps->data[i]) == index) {
            break;
        }
        i = i + 1;
    }
    return BluePrint_isUnlocked(((Array<BluePrint *> *)bluePrints)->data[i]);
}

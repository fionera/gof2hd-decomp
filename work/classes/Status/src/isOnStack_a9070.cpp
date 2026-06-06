#include "class.h"

extern "C" int Station_equals(Station *a, Station *b);

// Returns the matching stack slot's station (truthy) if `s` is already on the 3-deep
// station stack, else 0.  The original keeps this as a rolled loop over slots 0..2.
bool Status::isOnStack(Station *s) {
    unsigned i = 0;
    while (true) {
        if (2 < i) {
            return 0;
        }
        Station *cur = ((Station **)stationStack->data)[i];
        if (cur != 0 && Station_equals(cur, s) != 0) {
            break;
        }
        i = i + 1;
    }
    return ((int *)stationStack->data)[i];
}

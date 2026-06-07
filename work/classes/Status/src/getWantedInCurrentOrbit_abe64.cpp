#include "class.h"

extern "C" int Wanted_isActive(Wanted *w);
extern "C" int Wanted_isTerminated(Wanted *w);
extern "C" int Wanted_getCurrentLocation(Wanted *w);
extern "C" int Wanted_getRequiredBounties(Wanted *w);
extern "C" int Station_getIndex(Station *s);

// Returns the active, non-terminated wanted target located in the current orbit with the
// lowest required-bounty count.
Wanted *Status::getWantedInCurrentOrbit() {
    Array<Wanted *> *arr = wanted;
    Wanted *best = 0;
    if (arr != 0) {
        unsigned i = 0;
        while (i < arr->size) {
            if (Wanted_isActive(((Wanted **)wanted->data)[i]) != 0 &&
                Wanted_isTerminated(((Wanted **)wanted->data)[i]) == 0) {
                int loc = Wanted_getCurrentLocation(((Wanted **)wanted->data)[i]);
                if (loc == Station_getIndex(station)) {
                    if (best == 0 ||
                        Wanted_getRequiredBounties(best) <
                            Wanted_getRequiredBounties(((Wanted **)wanted->data)[i])) {
                        best = ((Wanted **)wanted->data)[i];
                    }
                }
            }
            i = i + 1;
            arr = wanted;
        }
    }
    return best;
}

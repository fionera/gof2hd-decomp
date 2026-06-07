#include "class.h"

extern "C" Station *Status_isOnStack(Status *self, Station *s);
extern "C" void Status_setStationTail(Status *self, Station *s);
extern "C" void *Station_dtor(Station *s);
extern "C" void operator_delete(void *p);

// Pushes `s` onto the 3-deep station stack (shifting older entries down), unless it is
// already present. Returns 1 if the stack was modified, 0 otherwise.
int Status::addStationToStack(Station *s) {
    Station *found = Status_isOnStack(this, s);
    if (found != 0) {
        Status_setStationTail(this, found);
        return 0;
    }
    int *base = (int *)stationStack->data;
    if (*base == 0) {
        int i = 0;
        int j;
        do {
            j = i;
            if (j + 2 < 0) {
                return 0;
            }
            i = j - 1;
        } while (base[j + 2] != 0);
        base[j + 2] = (int)(uint32_t)s;
    } else {
        int *p = base + 2;
        if ((Station *)*p != 0) {
            operator_delete(Station_dtor((Station *)*p));
            p = (int *)((int)stationStack->data + 8);
        }
        *p = 0;
        for (int i = 0; i != -2; i = i - 1) {
            int *slot = (int *)((int)stationStack->data + i * 4);
            slot[2] = slot[1];
        }
        *(int *)stationStack->data = (int)(uint32_t)s;
    }
    Status_setStationTail(this, s);
    return 1;
}

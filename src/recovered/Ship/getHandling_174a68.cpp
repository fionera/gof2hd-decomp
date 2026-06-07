#include "class.h"
extern "C" float getHandling(Ship *self) {
    int *m = (int *)self->mods;
    float h = 1.3f;
    if (m != 0) {
        int n = *m;
        for (int i = 0; n != i; i = i + 1) {
            if (((int *)m[1])[i] == 3) {
                h = h + 0.1f;
            }
        }
    }
    return h + self->handling;
}

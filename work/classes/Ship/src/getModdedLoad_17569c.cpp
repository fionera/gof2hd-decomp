#include "class.h"
extern "C" int getModdedLoad(Ship *self) {
    int load = self->baseLoad;
    int *m = (int *)self->mods;
    if (m == 0) return load;
    for (int i = 0; i < *m; i++)
        if (*(int *)(m[1] + i * 4) == 1) load += 0x1e;
    return load;
}

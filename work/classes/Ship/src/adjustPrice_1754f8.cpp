#include "class.h"
extern "C" int Status_getStation(void *s);
extern "C" void Status_getSystem(void *s);
extern "C" int SolarSystem_getRace(void);
extern int *gStatusRoot __attribute__((visibility("hidden")));
extern int *gShipDataRoot __attribute__((visibility("hidden")));
extern int *gRaceTable __attribute__((visibility("hidden")));
extern int *gDifficultyPtr __attribute__((visibility("hidden")));
extern "C" void adjustPrice(Ship *self) {
    int *status = gStatusRoot;
    if (Status_getStation(*(void **)status) != 0 && self->price > 0) {
        int *root = gShipDataRoot;
        int *table = *(int **)(*(int *)root + 4);
        int *entry = *(int **)((char *)table + self->index * 4);
        int cat = *entry;
        Status_getSystem(*(void **)status);
        int race = SolarSystem_getRace();
        int *table2 = *(int **)(*(int *)root + 4);
        int *entry2 = *(int **)((char *)table2 + self->index * 4);
        float base = (float)entry2[0x14 / 4];
        float f6 = 0.1f;
        if (gRaceTable[cat] == race) {
            f6 = base * 0.3f;
        }
        float acc = f6 + base;
        float f5 = 0.1f;
        int dv = *(int *)gDifficultyPtr;
        if (dv != 0) {
            f5 = base * (float)dv * 0.7f;
        }
        self->price = (int)(acc + f5);
    }
}

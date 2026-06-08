#include "class.h"

struct Status;
struct SolarSystem;
// Status singleton holder (PC-rel double-indirect): *g_apk_status -> Status*.
__attribute__((visibility("hidden"))) extern Status **g_apk_status;
// "Enemy race" lookup keyed by the current system's race (4 entries).
__attribute__((visibility("hidden"))) extern const int g_apk_raceTable[4];

extern "C" int Status_inAlienOrbit(Status *self);
extern "C" SolarSystem *Status_getSystem(Status *self);
extern "C" int SolarSystem_getRace();
extern "C" void Standing_applyPoints(Standing *self, int race, int delta);

// Standing::applyKill(int kind): a kill committed against another ship. For police
// kills (kind 8) the offence is mapped to the local system's owning race (once),
// otherwise it costs 5 standing toward the victim's faction.
extern "C" void Standing_applyKill(Standing *self, int kind) {
    Status **holder = g_apk_status;
    unsigned sysRace;
    if (Status_inAlienOrbit(*holder) != 0) {
        sysRace = 9;
    } else {
        Status_getSystem(*holder);
        sysRace = SolarSystem_getRace();
    }
    int delta;
    if (kind == 8) {
        if (self->f_4 >= 0) return;
        delta = 1;
        if (sysRace < 4) {
            kind = g_apk_raceTable[sysRace];
        } else {
            kind = 8;
        }
    } else {
        delta = 5;
    }
    Standing_applyPoints(self, kind, delta);
}

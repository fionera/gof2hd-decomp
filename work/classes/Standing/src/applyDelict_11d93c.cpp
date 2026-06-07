#include "class.h"

struct Status;
// Singleton holder: **g_adl_status -> the live Status object.
__attribute__((visibility("hidden"))) extern Status **g_adl_status;
extern "C" int Status_hardCoreMode(Status *self);
extern "C" void Standing_applyPoints(Standing *self, int race, int delta);

// Standing::applyDelict(int kind, int severity): a committed crime against a race.
// Severity doubles in hardcore mode; kinds 0/2 are "against the even slot", 1/3 the
// odd slot, with 0/2 applied as a penalty (negated) — all routed through applyPoints.
extern "C" void Standing_applyDelict(Standing *self, unsigned kind, int severity) {
    int hc = Status_hardCoreMode(*g_adl_status);
    int delta = severity << hc;
    int sign;
    switch (kind) {
    case 0:
        Standing_applyPoints(self, 0, -delta);
        return;
    case 1:
        Standing_applyPoints(self, 0, delta);
        return;
    case 2:
        Standing_applyPoints(self, 1, -delta);
        return;
    case 3:
        Standing_applyPoints(self, 1, delta);
        return;
    default:
        return;
    }
    (void)sign;
}

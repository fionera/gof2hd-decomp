#include "class.h"

// Toggles fast-forward mode. Only does work when the requested state differs from the
// current flag at 0x4d: re-derives the ship-handling damping coefficients (setShipHandling
// + two approximation passes) and records the new flag.
extern "C" void TFC_setShipHandling2(TargetFollowCamera *self, float v);
extern "C" void TFC_aproximate(void *out, float v, double *a, double *b,
                               double *c, double *d, double *e);

extern "C" void TFC_setFastForwardMode(TargetFollowCamera *self, bool on) {
    char *p = (char *)self;
    uint8_t cur = F<uint8_t>(self, 0x4d);
    if (on) {
        if (cur) return;
    } else {
        if (!cur) return;
    }
    TFC_setShipHandling2(self, F<float>(self, 0x134));
    TFC_aproximate(p + 0x80, F<float>(self, 0x128),
                   (double *)(p + 0x60), (double *)(p + 0x68),
                   (double *)(p + 0x70), (double *)(p + 0x78), (double *)(p + 0x80));
    TFC_aproximate(p + 0xa8, F<float>(self, 0x12c),
                   (double *)(p + 0x88), (double *)(p + 0x90),
                   (double *)(p + 0x98), (double *)(p + 0xa0), (double *)(p + 0xa8));
    F<uint8_t>(self, 0x4d) = on;
}

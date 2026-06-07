#include "class.h"

// Recomputes the two damping-coefficient sets by scaling the stored handling rates
// (0x128/0x12c) by `t` and feeding them through the approximation helper.
extern "C" void TFC_aproximate(void *out, float v, double *a, double *b,
                               double *c, double *d, double *e);

extern "C" void TFC_calculateCoefficents(TargetFollowCamera *self, float t) {
    char *p = (char *)self;
    TFC_aproximate(p + 0x80, F<float>(self, 0x128) * t,
                   (double *)(p + 0x60), (double *)(p + 0x68),
                   (double *)(p + 0x70), (double *)(p + 0x78), (double *)(p + 0x80));
    TFC_aproximate(p + 0xa8, F<float>(self, 0x12c) * t,
                   (double *)(p + 0x88), (double *)(p + 0x90),
                   (double *)(p + 0x98), (double *)(p + 0xa0), (double *)(p + 0xa8));
}

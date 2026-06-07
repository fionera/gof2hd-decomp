#include "class.h"

// ldrd r2,r3,[r1] ; ldr r1,[r1,#8] ; strd r2,r3,[r0,#8] ; str r1,[r0,#0x10]
// Copies the source Vector into the position slot at this+0x8.
extern "C" void TFC_setPositionV(TargetFollowCamera *self, Vector *src) {
    float a = src->x, b = src->y, c = src->z;
    F<float>(self, 0x08) = a;
    F<float>(self, 0x0c) = b;
    F<float>(self, 0x10) = c;
}

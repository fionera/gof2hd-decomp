#include "class.h"

// Sets the first-person flag at 0xf0, copies a fixed offset Vector {0,150,-800} into
// the slot at this+0xf4, and zeros 0x104/0x108. The Vector is staged in an on-stack
// byte buffer (engine source), so the compiler emits the stack canary automatically.
extern "C" void *Vector_assign(Vector *dst, void *src);

extern "C" void TFC_enableFirstPersonCam(TargetFollowCamera *self, bool on) {
    char buf[12];
    F<uint8_t>(self, 0xf0) = on;
    *(volatile float *)(buf + 4) = 150.0f;
    *(volatile float *)(buf + 0) = 0.0f;
    *(volatile float *)(buf + 8) = -800.0f;
    Vector_assign((Vector *)((char *)self + 0xf4), buf);
    F<int>(self, 0x104) = 0;
    F<int>(self, 0x108) = 0;
}

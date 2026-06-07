#include "class.h"

// Builds a local Vector {x,y,z} on the stack and copy-assigns it into the rotation
// slot at this+0x50. The engine's source stages the components in a small on-stack
// byte buffer, which makes the compiler emit the stack canary automatically (the
// push {r0-r3} / stm sp prologue + guard check).
extern "C" void *Vector_assign(Vector *dst, void *src);

extern "C" void TFC_rotateAroundTarget(TargetFollowCamera *self, float x, float y, float z) {
    char buf[12];
    __builtin_memcpy(buf + 0, &x, 4);
    __builtin_memcpy(buf + 4, &y, 4);
    __builtin_memcpy(buf + 8, &z, 4);
    Vector_assign((Vector *)((char *)self + 0x50), buf);
}

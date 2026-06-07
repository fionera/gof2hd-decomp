#include "class.h"

// adds r0,#0x2c ; b.w  — tail-call Vector copy-assign into the target-offset
// slot at this+0x2c (source Vector& stays in r1).
extern "C" void *Vector_assign(Vector *dst, Vector *src);

extern "C" void *TFC_setTargetOffset(TargetFollowCamera *self, Vector *v) {
    return Vector_assign((Vector *)((char *)self + 0x2c), v);
}

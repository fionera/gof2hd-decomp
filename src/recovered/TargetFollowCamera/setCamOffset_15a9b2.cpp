#include "class.h"

// Vector::operator=(this+0x38, v); this->[0xb0] = VectorLength(v).
extern "C" void *Vector_assign(Vector *dst, Vector *src);
extern "C" float AEMath_VectorLength(Vector *v);

extern "C" void TFC_setCamOffset(TargetFollowCamera *self, Vector *v) {
    Vector_assign((Vector *)((char *)self + 0x38), v);
    F<float>(self, 0xb0) = AEMath_VectorLength(v);
}

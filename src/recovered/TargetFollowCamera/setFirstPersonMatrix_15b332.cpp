#include "class.h"

// adds r0,#0xb4 ; b.w  — tail-call Matrix copy-assign into the first-person matrix
// slot at this+0xb4 (the source Matrix& stays in r1).
extern "C" void *Matrix_assign(Matrix *dst, Matrix *src);

extern "C" void *TFC_setFirstPersonMatrix(TargetFollowCamera *self, Matrix *m) {
    return Matrix_assign((Matrix *)((char *)self + 0xb4), m);
}

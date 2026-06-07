#include "class.h"

// setLocal(Matrix) takes the matrix by value (r0=this, the 16-word matrix in r1-r3 +
// stack) and copy-assigns it into the local-matrix slot at this+0x13c. The by-value
// aggregate is staged on the stack, so the compiler emits the canary automatically.
struct Mat64 { float m[16]; };
extern "C" void *Matrix_assign(void *dst, const void *src);

extern "C" void TFC_setLocal(TargetFollowCamera *self, Mat64 m) {
    Matrix_assign((char *)self + 0x13c, &m);
}

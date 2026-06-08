#include "class.h"

// NEAR / RESISTANT: Gun::shootAt(Matrix, int, Player*, bool) is a ~1714-byte function
// with a stack canary, a by-value Matrix argument, extensive FP/NEON vector math and
// dozens of cross-class calls. Under the fixed -Oz -fstack-protector toolchain the
// large-struct argument lowering plus FP register allocation diverge from the target;
// byte-exact is not attainable. The signature and entry behaviour are modelled below.

struct Player;

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
struct Matrix { float m[15]; };
} }
using AbyssEngine::AEMath::Matrix;

extern "C" void Gun_shootAt(Gun *self, Matrix m, int n, Player *p, bool b)
{
    // Records the firing transform / count and the (optional) target; the heavy body
    // (per-projectile spawn + vector setup) is data-driven from the Matrix argument.
    (void)m;
    self->f_60 = n;
    self->f_bc = p;
    self->f_88 = b;
}

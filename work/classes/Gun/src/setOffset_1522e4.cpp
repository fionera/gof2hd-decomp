#include "class.h"

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
using AbyssEngine::AEMath::Vector;

// AbyssEngine::AEMath::Vector::operator=(Vector*, Vector const&)
extern "C" void Vector_assign(Vector *dst, const Vector *src);

// Float offset applied to z (literal-pool constant in the target; not vmov-encodable).
static const float kZOffset = 0.1f;

extern "C" void Gun_setOffset(Gun *self, const Vector *v)
{
    char buf[12];
    Vector *local = (Vector *)buf;
    local->x = v->x;
    local->y = v->y;
    local->z = v->z + kZOffset;
    Vector_assign((Vector *)((char *)self + 0x7c), local);
}

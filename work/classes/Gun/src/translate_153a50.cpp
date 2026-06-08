#include "class.h"

namespace AbyssEngine { namespace AEMath {
struct Vector { float x, y, z; };
} }
using AbyssEngine::AEMath::Vector;

// AbyssEngine::AEMath::Vector::operator+=(Vector*, Vector const&)
extern "C" void Vector_addAssign(Vector *dst, const Vector *src);

extern "C" void Gun_translate(Gun *self, const Vector *v)
{
    int off = 0;
    for (unsigned i = 0; i < self->f_8; i = i + 1) {
        Vector_addAssign((Vector *)(self->f_c + off), v);
        off = off + 0xc;
    }
}

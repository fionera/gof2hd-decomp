#include "class.h"

extern "C" void AEGeometry_translate_vec(AEGeometry *self, const Vector *v);
extern "C" void Explosion_tail_translate(void *geometry, const Vector *v);

extern "C" void Explosion_translate(Explosion *self, const Vector *v)
{
    AEGeometry_translate_vec(G(self, 0x4), v);
    void *secondary = P(self, 0x8);
    if (secondary != 0) {
        return Explosion_tail_translate(secondary, v);
    }
}

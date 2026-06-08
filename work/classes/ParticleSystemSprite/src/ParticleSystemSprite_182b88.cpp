#include "class.h"

// ParticleSystemSprite::ParticleSystemSprite(PaintCanvas*, Matrix const*,
//     Array<ParticleSettings::ParticleSet> const&, bool, bool)
// Chains to the IParticleSystem base ctor, installs the derived vtable, allocates the per-
// particle sprite scratch array (count at +0x48, 12 bytes each) at +0x64 and zero-fills it,
// and caches a precomputed Pow value at +0x70.

struct ParticleSystemSprite;

void *operator new[](uint32_t size);
extern "C" void *memset(void *, int, unsigned long);

extern "C" void _ZN15IParticleSystemC2EPN11AbyssEngine10PaintCanvasEPKNS0_6AEMath6MatrixERK5ArrayI14ParticleSettings11ParticleSetEbb(
    void *self, void *canvas, const void *matrix, const void *sets, bool b4, bool b5);
extern "C" float _ZN11AbyssEngine6AEMath3PowEff(float base, float exp);
extern "C" char _ZTV20ParticleSystemSprite[];

extern "C" void *ParticleSystemSprite_ctor(
    void *self, void *canvas, const void *matrix, const void *sets, bool b4, bool b5)
{
    _ZN15IParticleSystemC2EPN11AbyssEngine10PaintCanvasEPKNS0_6AEMath6MatrixERK5ArrayI14ParticleSettings11ParticleSetEbb(
        self, canvas, matrix, sets, b4, b5);

    unsigned int count = F<unsigned int>(self, 0x48);

    // Install the derived vtable (base + 8 -> first virtual slot).
    *(void **)self = (void *)(_ZTV20ParticleSystemSprite + 8);

    void *arr = operator new[](count * 0xc);
    if (count != 0)
        memset(arr, 0, (unsigned long)count * 0xc);
    F<void *>(self, 0x64) = arr;

    F<float>(self, 0x70) = _ZN11AbyssEngine6AEMath3PowEff(0.0f, 0.0f);
    return self;
}

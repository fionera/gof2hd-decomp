#include "class.h"

extern "C" void _psm_emitTrail(ParticleSystemMesh *self, int id);
extern "C" void _psm_emitUsual(ParticleSystemMesh *self, int id);

extern "C" void _ZN18ParticleSystemMesh4emitEi(ParticleSystemMesh *self, int id)
{
    if (u8(self, 0xc) == 0 || u8(self, 0xd) == 0) {
        u8(self, 0x90) = 1;
        return;
    }

    uint32_t flags = u32(self, 0x34);
    if ((flags & 0x80) != 0)
        return;
    if ((int)(flags << 16) < 0)
        return _psm_emitTrail(self, id);
    return _psm_emitUsual(self, id);
}

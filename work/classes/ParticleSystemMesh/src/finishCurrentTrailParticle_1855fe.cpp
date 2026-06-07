#include "class.h"

struct ParticleSystemMesh {
    void finishCurrentTrailParticle(ParticleSet set, int id, const Vector &first, const Vector &second);
};

void ParticleSystemMesh::finishCurrentTrailParticle(ParticleSet set, int id, const Vector &first, const Vector &second)
{
    u8(ptr(this, 0x6c), id) = (uint8_t)set;
    i32(ptr(this, 0x68), (uint32_t)id * 4) = 0;

    uint32_t flags = u32(this, 0x34);
    uint32_t offset = (u32(this, 0x98) * id * 2) | 1;
    Vector *dst = (Vector *)((char *)ptr(this, 0x64) + offset * 12);

    if ((flags & 0x1000) != 0) {
        *dst = first;
        flags = u32(this, 0x34);
        dst = (Vector *)((char *)dst + 0x18);
    }
    if ((int)(flags << 18) < 0)
        *dst = second;
}

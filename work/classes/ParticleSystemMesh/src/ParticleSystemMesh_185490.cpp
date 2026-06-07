#include "class.h"

extern "C" void _psm_base_ctor(ParticleSystemMesh *self, PaintCanvas *canvas, const Matrix *matrix, const void *sets,
                               bool a, bool b);
extern "C" uint32_t g_ParticleSystemMesh_vtable[];
extern "C" char g_ParticleSetData[];
extern "C" void __aeabi_memclr4(void *dst, uint32_t size);

struct ParticleSystemMesh {
    ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const void *sets, bool a, bool b);
};

ParticleSystemMesh::ParticleSystemMesh(PaintCanvas *canvas, const Matrix *matrix, const void *sets, bool a, bool b)
{
    _psm_base_ctor(this, canvas, matrix, sets, a, b);

    uint32_t flags = u32(this, 0x34);
    uint32_t particleCount = u32(this, 0x48);
    u32(this, 0x80) = 0;
    u32(this, 0x84) = 0;
    u32(this, 0x88) = 0;

    uint32_t edgeCount = ((flags >> 13) & 1) + ((flags >> 12) & 1);
    if ((flags & 0x4000) != 0)
        edgeCount++;

    *(uint32_t **)this = g_ParticleSystemMesh_vtable + 2;

    uint32_t wide = (flags >> 16) & 1;
    uint32_t stride = edgeCount << wide;
    u8(this, 0x74) = (uint8_t)wide;
    uint32_t quads = particleCount * stride;
    u32(this, 0x98) = edgeCount;
    u32(this, 0x9c) = stride;
    u32(this, 0x70) = quads << 2;

    void *positions;
    uint32_t clearSize;
    uint32_t clearCount;
    if ((flags & 0x8000) == 0) {
        uint64_t bytes = (uint64_t)particleCount * 12u;
        uint32_t size = (uint32_t)bytes;
        if ((uint32_t)(bytes >> 32) != 0)
            size = 0xffffffff;
        positions = operator new(size);
        clearCount = particleCount;
        clearSize = particleCount * 12u;
    } else {
        if (u32((void *)sets, 0) != 0) {
            int set = *(int *)ptr((void *)sets, 4);
            if (set != -1) {
                char *data = g_ParticleSetData + (set * 160);
                if (*(float *)(data + 0x40) > 0.0f)
                    u32(this, 0x70) = (quads << 2) + (stride << 2);
            }
        }

        uint32_t vectorCount = particleCount * edgeCount * 2;
        uint64_t bytes = (uint64_t)vectorCount * 12u;
        uint32_t size = (uint32_t)bytes;
        if ((uint32_t)(bytes >> 32) != 0)
            size = 0xffffffff;
        positions = operator new(size);
        clearCount = vectorCount;
        clearSize = particleCount * edgeCount * 24u;
    }

    if (clearCount != 0) {
        uint32_t n = clearSize - 12u;
        uint32_t rem = n % 12u;
        __aeabi_memclr4(positions, (n - rem) + 12u);
    }

    ptr(this, 0x64) = positions;
    u32(this, 0x78) = 0;
    u32(this, 0x7c) = 0;
}

#include "class.h"

extern "C" void _psm_meshSetPoint(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y, float z);

struct ParticleSystemMesh {
    void reset();
};

void ParticleSystemMesh::reset()
{
    for (int i = 0; i < i32(this, 0x70); i++) {
        _psm_meshSetPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54),
                          (uint16_t)(u32(this, 0x58) + i), 0.0f, 0.0f, 0.0f);
    }

    for (int i = 0; i < i32(this, 0x48); i++)
        i32(ptr(this, 0x68), (uint32_t)i * 4) = -1;

    *(volatile uint32_t *)((char *)this + 0x94) = 0;
    *(volatile uint32_t *)((char *)this + 0x50) = 0;
    *(volatile uint8_t *)((char *)this + 0x90) = 1;
    *(volatile uint32_t *)((char *)this + 0x60) = 0;
    *(volatile uint8_t *)((char *)this + 0x4) = 1;
}

#include "class.h"

extern "C" void _psm_meshSetUV(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float u, float v);
extern "C" void _psm_meshSetZero(PaintCanvas *canvas, uint32_t mesh, uint16_t point, uint32_t value);
extern "C" void _psm_meshSetTriangle(PaintCanvas *canvas, uint32_t mesh, uint16_t triangle, uint16_t a, uint16_t b,
                                     uint16_t c);

struct ParticleSystemMesh {
    int init(uint32_t mesh, uint32_t firstPoint);
};

int ParticleSystemMesh::init(uint32_t mesh, uint32_t firstPoint)
{
    u32(this, 0x54) = mesh;
    u32(this, 0x58) = firstPoint;

    void (*setUV)(PaintCanvas *, uint32_t, uint16_t, float, float) = _psm_meshSetUV;
    void (*setZero)(PaintCanvas *, uint32_t, uint16_t, uint32_t) = _psm_meshSetZero;

    for (int i = 0; i < i32(this, 0x70); i += 4) {
        setUV((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i), 0.0f, 0.0f);
        setUV((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 1), 1.0f, 0.0f);
        setUV((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 2), 0.0f, 1.0f);
        setUV((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 3), 1.0f, 1.0f);
        setZero((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i), 0);
        setZero((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 1), 0);
        setZero((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 2), 0);
        setZero((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(u32(this, 0x58) + i + 3), 0);
    }

    int point = i32(this, 0x58);
    for (int tri = 0; tri < (i32(this, 0x70) >> 1); tri += 2) {
        _psm_meshSetTriangle((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54),
                             (uint16_t)(tri + (u32(this, 0x58) >> 1)), (uint16_t)(point + 2),
                             (uint16_t)(point + 1), (uint16_t)point);
        _psm_meshSetTriangle((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54),
                             (uint16_t)(tri + (u32(this, 0x58) >> 1) + 1), (uint16_t)(point + 1),
                             (uint16_t)(point + 2), (uint16_t)(point + 3));
        point += 4;
    }

    u8(this, 0x5c) = 1;
    return (*(int (**)(ParticleSystemMesh *))(*(uint32_t *)this + 8))(this);
}

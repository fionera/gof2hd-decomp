#include "class.h"

extern "C" void _psm_interpolateColor(ParticleSystemMesh *self, int id, float *b, float *g, float *a, float *r);
extern "C" void _psm_meshSetColor(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float a, float r, float g,
                                  float b);
extern "C" char g_ParticleSetData[];

struct ParticleSystemMesh {
    void updateSingleColor(int id);
};

void ParticleSystemMesh::updateSingleColor(int id)
{
    float b;
    float g;
    float r;
    float a;

    int start = i32(this, 0x58);
    int stride = i32(this, 0x9c);
    if ((int)(u8(this, 0x35) << 24) < 0) {
        int prev = id == 0 ? i32(this, 0x48) : id;
        if (i32(ptr(this, 0x68), (uint32_t)(prev - 1) * 4) == -1) {
            int set = *(signed char *)((char *)ptr(this, 0x6c) + id);
            uint32_t color = *(uint32_t *)(g_ParticleSetData + set * 160 + 0x38);
            uint32_t mask = u8(this, 0x45) == 0 ? 0xffffff00u : 0xffu;
            color &= mask;
            r = (float)((color >> 16) & 0xff) * 0.0039215689f;
            a = (float)(color >> 24) * 0.0039215689f;
            g = (float)((color >> 8) & 0xff) * 0.0039215689f;
            b = (float)(color & 0xff) * 0.0039215689f;
        } else {
            _psm_interpolateColor(this, id, &b, &g, &a, &r);
        }
    } else {
        _psm_interpolateColor(this, id, &b, &g, &a, &r);
    }

    int point = start + stride * id * 4;
    for (int i = 0; i < i32(this, 0x9c); i++) {
        _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 2), a, r, g, b);
        _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 3), a, r, g, b);
        point += 4;
    }

    if ((int)(u8(this, 0x35) << 24) < 0) {
        int next = (id == i32(this, 0x48) - 1) ? 0 : id + 1;
        if (i32(ptr(this, 0x68), (uint32_t)next * 4) == -1)
            return;
        point = i32(this, 0x58) + i32(this, 0x9c) * next * 4;
        for (int i = 0; i < i32(this, 0x9c); i++) {
            _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)point, a, r, g, b);
            _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 1), a, r, g, b);
            point += 4;
        }
    } else {
        point = start + stride * id * 4;
        for (int i = 0; i < i32(this, 0x9c); i++) {
            _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)point, a, r, g, b);
            _psm_meshSetColor((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 1), a, r, g, b);
            point += 4;
        }
    }
}

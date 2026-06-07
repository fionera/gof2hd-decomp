#include "class.h"

extern "C" void _psm_vectorScale(Vector *out, const Vector *v, float scale);
extern "C" void _psm_meshTranslatePoint(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y, float z);

struct ParticleSystemMesh {
    void updateUsualEdges(int id, int delta);
};

void ParticleSystemMesh::updateUsualEdges(int id, int delta)
{
    Vector move;
    Vector tmp;
    float scale = (float)delta * 0.001f;
    const Vector *src;
    if ((int)(u8(this, 0x36) << 28) < 0) {
        const Vector *trail = (const Vector *)((char *)ptr(this, 0x64) + id * 12);
        src = (const Vector *)((char *)this + 0x1c);
        scale *= trail->y;
    } else {
        src = (const Vector *)((char *)ptr(this, 0x64) + id * 12);
    }

    _psm_vectorScale(&tmp, src, scale);
    move = tmp;

    int point = i32(this, 0x58) + i32(this, 0x9c) * id * 4;
    for (int i = 0; i < i32(this, 0x9c) * 4; i++)
        _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + i), move.x,
                                move.y, move.z);
}

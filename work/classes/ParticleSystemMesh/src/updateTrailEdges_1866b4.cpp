#include "class.h"

extern "C" void _psm_vectorScale(Vector *out, const Vector *v, float scale);
extern "C" void _psm_meshTranslatePoint(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y, float z);

struct ParticleSystemMesh {
    void updateTrailEdges(int id, int delta);
};

void ParticleSystemMesh::updateTrailEdges(int id, int delta)
{
    int edgeCount = i32(this, 0x98);
    int stride = i32(this, 0x9c);
    int point = i32(this, 0x58) + id * stride * 4;
    Vector *edge = (Vector *)((char *)ptr(this, 0x64) + id * edgeCount * 24);
    float scale = (float)delta * 0.001f;

    for (int i = 0; i < edgeCount; i++) {
        Vector move;
        _psm_vectorScale(&move, edge, scale);
        _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)point, -move.x, move.y,
                                -move.z);
        int span = u8(this, 0x74) == 0 ? 1 : 4;
        _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + span), move.x,
                                move.y, move.z);

        if (i32(ptr(this, 0x68), (uint32_t)id * 4) != -2 || (int)(u8(this, 0x35) << 24) >= 0) {
            Vector move2;
            _psm_vectorScale(&move2, edge + 1, scale);
            _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 2),
                                    -move2.x, move2.y, -move2.z);
            _psm_meshTranslatePoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + span + 2),
                                    move2.x, move2.y, move2.z);
            edge += 2;
            point += u8(this, 0x74) == 0 ? 4 : 8;
        } else {
            edge += 1;
        }
    }
}

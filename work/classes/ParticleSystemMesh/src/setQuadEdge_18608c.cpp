#include "class.h"

extern "C" void _psm_vectorMinus(Vector *out, const Vector *a, const Vector *b);
extern "C" void _psm_vectorPlus(void *out, const Vector *a, const Vector *b);
extern "C" void _psm_meshSetPoint(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y, float z);
extern "C" void _psm_meshSetPointIndirect(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y,
                                          float z);

struct ParticleSystemMesh {
    void setQuadEdge(const Vector &edge, int point, const Vector &delta);
};

void ParticleSystemMesh::setQuadEdge(const Vector &edge, int point, const Vector &delta)
{
    Vector pos;
    volatile char tmpStorage[sizeof(Vector)];

    _psm_vectorMinus(&pos, &edge, &delta);
    _psm_meshSetPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)point, pos.x, pos.y, pos.z);

    _psm_vectorPlus((void *)tmpStorage, &edge, &delta);
    pos = *(Vector *)(void *)tmpStorage;

    uint8_t wide = u8(this, 0x74);
    int next = point + 1;
    PaintCanvas *canvas = (PaintCanvas *)ptr(this, 0x8);
    uint32_t mesh = u32(this, 0x54);
    if (wide == 0) {
        _psm_meshSetPoint(canvas, mesh, (uint16_t)next, pos.x, pos.y, pos.z);
    } else {
        void (*setPoint)(PaintCanvas *, uint32_t, uint16_t, float, float, float) = _psm_meshSetPointIndirect;
        setPoint(canvas, mesh, (uint16_t)next, edge.x, edge.y, edge.z);
        setPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 5), edge.x, edge.y, edge.z);
        setPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + 4), pos.x, pos.y, pos.z);
    }
}

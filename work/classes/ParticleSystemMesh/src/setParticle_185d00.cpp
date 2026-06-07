#include "class.h"

extern "C" void _psm_matrixGetRight(Vector *out, const Matrix *m);
extern "C" void _psm_matrixGetUp(Vector *out, const Matrix *m);
extern "C" void _psm_matrixGetDir(Vector *out, const Matrix *m);
extern "C" void _psm_vectorScale(Vector *out, const Vector *v, float scale);
extern "C" void _psm_vectorMinus(Vector *out, const Vector *a, const Vector *b);
extern "C" void _psm_vectorPlus(Vector *out, const Vector *a, const Vector *b);
extern "C" void _psm_vectorAssign(Vector *dst, const Vector *src);
extern "C" void _psm_setQuadEdge(ParticleSystemMesh *self, const Vector *edge, int point, const Vector *delta);
extern "C" void _psm_meshSetColorWord(PaintCanvas *canvas, uint32_t mesh, uint16_t point, uint32_t color);
extern "C" void _psm_meshSetUV2(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float u, float v);

struct ParticleSystemMesh {
    void setParticle(const Vector &pos, float scale, uint32_t color, float u0, float u1, float v0, float v1,
                     bool useMaskedColor, float upScale, float dirScale, const Vector &delta, bool finish);
};

void ParticleSystemMesh::setParticle(const Vector &pos, float scale, uint32_t color, float u0, float u1, float v0,
                                     float v1, bool useMaskedColor, float upScale, float dirScale,
                                     const Vector &delta, bool finish)
{
    Vector right;
    Vector rightScaled;
    Vector up;
    Vector upScaled;
    Vector dir;
    Vector dirScaled;
    Vector tmpA;
    Vector tmpB;

    _psm_matrixGetRight(&right, (const Matrix *)ptr(this, 0x18));
    _psm_vectorScale(&rightScaled, &right, scale);
    if (u8(this, 0x4c) != 0) {
        _psm_vectorScale(&tmpA, &rightScaled, -1.0f);
        _psm_vectorAssign(&rightScaled, &tmpA);
    }

    _psm_matrixGetUp(&up, (const Matrix *)ptr(this, 0x18));
    _psm_vectorScale(&upScaled, &up, upScale == 0.0f ? (float)useMaskedColor : upScale);
    _psm_matrixGetDir(&dir, (const Matrix *)ptr(this, 0x18));
    _psm_vectorScale(&dirScaled, &dir, dirScale == 0.0f ? scale : dirScale);

    uint32_t flags = u32(this, 0x34);
    if ((flags & 0x20000) != 0) {
        _psm_vectorMinus(&tmpA, &upScaled, &rightScaled);
        _psm_vectorScale(&dirScaled, &tmpA, 0.70710677f);
        _psm_vectorPlus(&tmpB, &rightScaled, &upScaled);
        _psm_vectorScale(&rightScaled, &tmpB, 0.70710677f);
        flags = u32(this, 0x34);
    }

    int point = i32(this, 0x58) + i32(this, 0x9c) * i32(this, 0x50) * 4;
    if ((int)(flags << 19) < 0) {
        _psm_vectorMinus(&tmpA, &pos, &upScaled);
        _psm_vectorMinus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point, &rightScaled);
        _psm_vectorPlus(&tmpA, &pos, &upScaled);
        _psm_vectorPlus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point + 2, &rightScaled);
        point += u8(this, 0x74) == 0 ? 4 : 8;
        flags = u32(this, 0x34);
    }
    if ((int)(flags << 18) < 0) {
        _psm_vectorMinus(&tmpA, &pos, &upScaled);
        _psm_vectorPlus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point, &dirScaled);
        _psm_vectorPlus(&tmpA, &pos, &upScaled);
        _psm_vectorMinus(&tmpB, &tmpA, &delta);
        _psm_setQuadEdge(this, &tmpB, point + 2, &dirScaled);
        point += u8(this, 0x74) == 0 ? 4 : 8;
        flags = u32(this, 0x34);
    }
    if ((int)(flags << 17) < 0) {
        _psm_vectorPlus(&tmpB, &pos, &rightScaled);
        _psm_setQuadEdge(this, &tmpB, point, &upScaled);
        _psm_vectorMinus(&tmpB, &pos, &rightScaled);
        _psm_setQuadEdge(this, &tmpB, point + 2, &upScaled);
    }

    uint32_t frontColor = color;
    uint32_t backColor = color;
    uint32_t mask = u8(this, 0x45) == 0 ? 0xffffff00u : 0xffu;
    if (useMaskedColor && !finish)
        frontColor &= mask;
    if (finish)
        backColor &= mask;

    int base = i32(this, 0x58) + i32(this, 0x9c) * i32(this, 0x50) * 4;
    for (int i = 0; i < i32(this, 0x9c); i++) {
        _psm_meshSetColorWord((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)base, backColor);
        _psm_meshSetColorWord((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 1), backColor);
        _psm_meshSetColorWord((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 2), frontColor);
        _psm_meshSetColorWord((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 3), frontColor);
        _psm_meshSetUV2((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)base, u0, v0);
        _psm_meshSetUV2((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 1), u1, v0);
        _psm_meshSetUV2((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 2), u0, v1);
        _psm_meshSetUV2((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(base + 3), u1, v1);
        base += 4;
    }
}

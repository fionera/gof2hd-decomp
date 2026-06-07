#include "class.h"

extern "C" void _psm_updateUsualEdges(ParticleSystemMesh *self, int id, int delta);
extern "C" void _psm_updateTrailEdges(ParticleSystemMesh *self, int id, int delta);
extern "C" void _psm_updateSingleColor(ParticleSystemMesh *self, int id);
extern "C" void _psm_finishCurrentTrailParticle(ParticleSystemMesh *self, ParticleSet set, int id, const Vector *a,
                                                const Vector *b);
extern "C" void _psm_matrixGetRight(Vector *out, const Matrix *m);
extern "C" void _psm_matrixGetUp(Vector *out, const Matrix *m);
extern "C" void _psm_vectorScale(Vector *out, const Vector *v, float scale);
extern "C" char g_ParticleSetData[];
extern "C" void _psm_meshSetPoint(PaintCanvas *canvas, uint32_t mesh, uint16_t point, float x, float y, float z);

struct ParticleSystemMesh {
    void updateSingle(int id, float delta);
};

void ParticleSystemMesh::updateSingle(int id, float delta)
{
    int intDelta = (int)delta;
    int set = *(signed char *)((char *)ptr(this, 0x6c) + id);
    if ((int)(u8(this, 0x35) << 24) < 0) {
        _psm_updateTrailEdges(this, id, intDelta);
        if (i32(ptr(this, 0x68), (uint32_t)id * 4) == -2 && u8(this, 0x90) != 0) {
            Vector right;
            Vector scaledRight;
            Vector up;
            Vector scaledUp;
            _psm_matrixGetRight(&right, (const Matrix *)ptr(this, 0x18));
            _psm_vectorScale(&scaledRight, &right, u8(this, 0x4c) == 0 ? 1.0f : -1.0f);
            _psm_matrixGetUp(&up, (const Matrix *)ptr(this, 0x18));
            float s = (float)*(int32_t *)(g_ParticleSetData + set * 160 + 0x44);
            _psm_vectorScale(&scaledRight, &scaledRight, s);
            _psm_vectorScale(&scaledUp, &up, s);
            _psm_finishCurrentTrailParticle(this, (ParticleSet)set, id, &scaledRight, &scaledUp);
        }
    } else {
        _psm_updateUsualEdges(this, id, intDelta);
    }

    int age = i32(ptr(this, 0x68), (uint32_t)id * 4);
    age = (int)((float)age + delta);
    i32(ptr(this, 0x68), (uint32_t)id * 4) = age;
    _psm_updateSingleColor(this, id);

    int lifetime = *(int32_t *)(g_ParticleSetData + set * 160 + 0x28);
    if (age > lifetime) {
        i32(ptr(this, 0x68), (uint32_t)id * 4) = -1;
        int point = i32(this, 0x58) + i32(this, 0x9c) * id * 4;
        for (int i = 0; i < i32(this, 0x9c) * 4; i++)
            _psm_meshSetPoint((PaintCanvas *)ptr(this, 0x8), u32(this, 0x54), (uint16_t)(point + i), 0.0f, 0.0f,
                              0.0f);
    }
}

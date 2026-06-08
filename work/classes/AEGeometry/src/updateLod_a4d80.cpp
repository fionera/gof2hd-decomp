#include "class.h"

using namespace AbyssEngine;
using namespace AbyssEngine::AEMath;

// AEGeometry::updateLod(const Vector& camPos, float screenScale)
//   Recomputes the camera-distance of this object, decides whether it is within
//   its last-visible range, and -- if so -- selects the appropriate LOD level by
//   comparing the squared distance against each level's threshold (scaled by a
//   detail factor derived from the screen scale), swapping in that level's local
//   matrix and notifying the LodMeshMerger.
extern "C" void *__aeabi_memcpy_b(void *dst, const void *src, uint32_t n);     // 0x6f1e4
extern "C" Vector AEMath_MatrixGetPosition_ret(const void *m);                 // 0x6f16c
extern "C" unsigned long long __aeabi_f2ulz_(float f);                          // 0x73078
extern "C" float __aeabi_ul2f_(unsigned long long v);                          // 0x73084
extern "C" uint32_t Transform_GetTransform(uint32_t tf);                       // 0x72088
extern "C" void Transform_SetCurrentAnimationTime(uint32_t t, uint32_t a, uint32_t b); // 0x73090
extern "C" void LodMeshMerger_setLod(void *lmm, uint32_t idx, int lod);        // 0x7309c
extern "C" void _ae_matrix_assign(void *dst, const void *src);                 // 0x6f148
extern "C" void _ae_vector_assign(void *dst, const void *src);                 // 0x6eb3c

struct AEGeometry { void updateLod(const Vector &camPos, float screenScale); };

void AEGeometry::updateLod(const Vector &camPos, float screenScale)
{
    this->f_48 = this->f_49;

    char matrixCopy[60];
    uint32_t loc = PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    __aeabi_memcpy_b(matrixCopy, (void *)loc, 0x3c);

    loc = PaintCanvas::TransformGetLocal(this->f_2c, this->f_c);
    Vector pos = AEMath_MatrixGetPosition_ret((void *)loc);
    Vector delta = AbyssEngine::AEMath::operator-(camPos, pos);
    _ae_vector_assign((char *)this + 0x78, &delta);

    float dx = this->f_78, dy = this->f_7c, dz = this->f_80;
    unsigned long long distSq = __aeabi_f2ulz_(dy * dy + dx * dx + dz * dz);
    *(unsigned long long *)((char *)this + 0x68) = distSq;

    unsigned long long lastVis = *(unsigned long long *)((char *)this + 0x70);
    bool visible;
    if (lastVis == 0) {
        visible = true;            // no clamp configured -> always considered visible
    } else {
        visible = distSq < lastVis;
        this->f_48 = visible ? 1 : 0;
    }

    if (!visible) {
        this->f_28 = -1;
        return;
    }

    Transform_GetTransform(this->f_2c);

    // Detail factor: tighter LOD thresholds when the object is small on screen.
    float factor = (screenScale <= 0.0625f) ? 0.75f : 1.0f;
    float detail = (0.03125f < screenScale) ? factor : 0.5f;

    int count = this->f_50;
    int level = count;
    char *distances = (char *)this->f_64;   // per-level squared distance (u64 each)

    // Walk levels high->low; stop at the first whose scaled threshold is met.
    while (level >= 1) {
        int idx = level - 1;
        float thresh = __aeabi_ul2f_(*(unsigned long long *)(distances + idx * 8));
        float d = __aeabi_ul2f_(*(unsigned long long *)((char *)this + 0x68));
        if (!(detail * thresh < d)) {
            level = idx;
            continue;
        }
        // This level is the one to use.
        void **lodMats = (void **)this->f_54;
        void *lodMat = lodMats[idx];
        if (lodMat != this->f_c) {
            PaintCanvas::TransformSetLocal((PaintCanvas *)(unsigned long)this->f_2c,
                                           this->f_c, (Matrix *)lodMat);
            this->f_c = ((void **)this->f_54)[idx];
            uint32_t t = Transform_GetTransform(this->f_2c);
            Transform_SetCurrentAnimationTime(t, 0, 0);
            t = Transform_GetTransform(this->f_2c);
            Transform_SetCurrentAnimationTime(t, 0, 0);
            this->f_28 = level;
            _ae_matrix_assign((char *)this + 0x84, matrixCopy);
            void *lmm = this->f_4;
            if (lmm != 0)
                LodMeshMerger_setLod(lmm, this->f_0, (signed char)level);
        }
        return;
    }

    // No level matched: fall back to the base mesh.
    PaintCanvas::TransformSetLocal((PaintCanvas *)(unsigned long)this->f_2c,
                                   this->f_c, (Matrix *)this->f_18);
    this->f_28 = 0;
    this->f_c = this->f_18;
    void *lmm = this->f_4;
    if (lmm != 0)
        LodMeshMerger_setLod(lmm, this->f_0, 0);
}

#include "class.h"

extern "C" {
void *operator_new_array(uint32_t size);                          // 0x6ec08
void *operator_new_mesh(uint32_t size);                           // 0x6eb24 (operator new)
// AEMath helpers.
void AEMath_MatrixTransformVector(Vector *out, const Matrix &m, const Vector &v);  // 0x6f688
void AEMath_MatrixRotateVector(Vector *out, const Matrix &m, const Vector &v);     // 0x6f694
void AEMath_VectorNormalize(Vector *out, const Vector &v);                          // 0x6ec80
void AEMath_VectorAssign(Vector *dst, const Vector *src);                           // 0x6eb3c
void AEMath_BSphereAssign(void *dst, const void *src);                              // 0x6eb18
}

// LodMeshMerger::transformMesh(AbyssEngine::Mesh* src, AbyssEngine::AEMath::Matrix const& m)
//   Builds a fresh Mesh that is `src` with all geometry transformed by `m`: positions are
//   transformed, normals rotated+normalized, and the bounding sphere is recomputed.
void *LodMeshMerger::transformMesh(Mesh *src, const Matrix &m)
{
    char *s = (char *)src;
    char *out = (char *)operator_new_mesh(0x88);

    // zero-initialise the new mesh (the engine writes a zero vector + identity-ish defaults).
    for (uint32_t off = 0; off < 0x88; off += 4)
        *(uint32_t *)(out + off) = 0;
    *(float *)(out + 0x4c) = 1.0f;   // bsphere radius default = 1.0

    // copy counts/flags from source.
    uint32_t vcount = *(uint16_t *)(s + 0x2);
    *(uint16_t *)(out + 0x2) = *(uint16_t *)(s + 0x2);
    *(uint16_t *)(out + 0x28) = *(uint16_t *)(s + 0x28);
    uint8_t flags = *(uint8_t *)s;
    *(uint8_t *)out = flags;

    // conditional pointer copies based on flag bits (texcoords / colours / indices).
    if (flags & 0x2)
        *(uint32_t *)(out + 0x8) = *(uint32_t *)(s + 0x8);
    if (flags & 0x10)   // bit 4 (uVar9 << 0x1c < 0)
        *(uint32_t *)(out + 0xc) = *(uint32_t *)(s + 0xc);
    if (flags & 0x20)   // bit 5 (uVar9 << 0x1b < 0)
        *(uint32_t *)(out + 0x2c) = *(uint32_t *)(s + 0x2c);

    // --- transform vertex positions (flag bit 0) ---------------------------
    if (flags & 0x1) {
        char *dstV = (char *)operator_new_array(vcount * 0xc);
        *(void **)(out + 0x4) = dstV;
        int o = 0;
        for (uint32_t i = 0; i < vcount; i = i + 1) {
            Vector tmp;
            AEMath_MatrixTransformVector(&tmp, m, *(Vector *)(*(char **)(s + 0x4) + o));
            AEMath_VectorAssign((Vector *)(*(char **)(out + 0x4) + o), &tmp);
            o = o + 0xc;
            vcount = *(uint16_t *)(s + 0x2);
        }
        flags = *(uint8_t *)s;
    }

    // --- rotate + normalise normals (flag bit 5 / << 0x1d) -----------------
    if (flags & 0x4) {
        char *dstN = (char *)operator_new_array(vcount * 0xc);
        *(void **)(out + 0x10) = dstN;
        int o = 0;
        for (uint32_t i = 0; i < vcount; i = i + 1) {
            Vector rot;
            AEMath_MatrixRotateVector(&rot, m, *(Vector *)(*(char **)(s + 0x10) + o));
            Vector nrm;
            AEMath_VectorNormalize(&nrm, rot);
            AEMath_VectorAssign((Vector *)(*(char **)(out + 0x10) + o), &nrm);
            o = o + 0xc;
            vcount = *(uint16_t *)(s + 0x2);
        }
    }

    // --- recompute bounding sphere -----------------------------------------
    // transform the centre, then derive a conservative radius from the transformed extents.
    float r = *(float *)(s + 0x48);
    Vector ext = { r, r, r };
    Vector tExt;
    AEMath_MatrixTransformVector(&tExt, m, ext);

    Vector center;
    AEMath_MatrixTransformVector(&center, m, *(Vector *)(s + 0x3c));

    // pick the largest |component| of the transformed extent as the new radius.
    float ax = tExt.x < 0.0f ? -tExt.x : tExt.x;
    float ay = tExt.y < 0.0f ? -tExt.y : tExt.y;
    float az = tExt.z < 0.0f ? -tExt.z : tExt.z;
    float rad = ax;
    if (ay > rad) rad = ay;
    if (az > rad) rad = az;

    // assemble the BSphere { center.xyz, radius } and store via operator=.
    struct BSphere { float x, y, z, r; } sph;
    sph.x = center.x;
    sph.y = center.y;
    sph.z = center.z;
    sph.r = rad;
    AEMath_BSphereAssign(out + 0x3c, &sph);

    return out;
}

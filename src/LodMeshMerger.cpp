#include "gof2/LodMeshMerger.h"
#include "gof2/Vector.h"


extern "C" void LodMeshMerger_setMatrix_tail(void *dst, const Matrix &m);
extern "C" void PaintCanvas_MeshCreate(void *canvas, uint16_t meshId, uint32_t *out, bool flag);
extern "C" void *PaintCanvas_MeshGetPointer(void *canvas, uint32_t id);
extern "C" uint8_t PaintCanvas_CameraIsSphereInViewFrustum(void *canvas, const Vector *center, float r);

// ---- setEnabled_181438.cpp ----
void LodMeshMerger::setEnabled(int index, bool enabled)
{
    uint8_t *flags = field_0x30;
    if (flags[index] != enabled) {
        flags[index] = enabled;
        if (((int8_t *)field_0x34)[index] == 0) {
            return;
        }
        field_0x3c = 1;
    }
}

// ---- setLod_181400.cpp ----
void LodMeshMerger::setLod(int index, signed char lod)
{
    int8_t *lods = field_0x2c;
    if (lods[index] != lod) {
        lods[index] = lod;
        if (((int8_t *)field_0x30)[index] == 0) {
            return;
        }
        field_0x3c = 1;
    }
}

// ---- setMatrix_181428.cpp ----
// setMatrix(index, m): tail-call the engine matrix-assign with the per-index
// slot (matrices[index], each 0x3c bytes) and the source matrix.

void LodMeshMerger::setMatrix(int index, const Matrix &m)
{
    char *base = (char *)field_0x28;
    return LodMeshMerger_setMatrix_tail(base + index * 0x3c, m);
}

// ---- setMesh_1813ac.cpp ----
void LodMeshMerger::setMesh(int index, signed char lod, uint16_t meshId)
{
    uint32_t id;
    PaintCanvas_MeshCreate(field_0x14, meshId, &id, false);
    void *ptr = PaintCanvas_MeshGetPointer(field_0x14, id);
    AEMesh **meshes = field_0x8.data();
    meshes[field_0x0 * lod + index] = (AEMesh *)ptr;
}

// ---- update_18174c.cpp ----
extern "C" void aeabi_memcpy4(void *dst, const void *src, uint32_t n);  // __aeabi_memcpy4

void LodMeshMerger::update()
{
    int rows = field_0x0;
    for (int i = 0; i < rows; i++) {
        void *sph = field_0x24[i];
        uint8_t vis = PaintCanvas_CameraIsSphereInViewFrustum(
            field_0x14,
            (const Vector *)((char *)sph + 0x3c),
            *(float *)((char *)sph + 0x48));
        int8_t *visArr = (int8_t *)field_0x34;
        if (vis != (uint8_t)visArr[i]) {
            visArr[i] = (int8_t)vis;
            if (((int8_t *)field_0x30)[i] != 0) {
                field_0x3c = 1;
            }
        }
        rows = field_0x0;
    }

    if (field_0x3c != 0) {
        int sumIdx = 0;   // r6 (index/triangle accumulator for the budget test)
        for (int j = 0; j < rows; j++) {
            if (((int8_t *)field_0x30)[j] != 0 &&
                ((int8_t *)field_0x34)[j] != 0) {
                signed char lod = ((int8_t *)field_0x2c)[j];
                AEMesh *src = (AEMesh *)field_0x24[rows * lod + j];
                sumIdx += (uint16_t)src->field_0x28;
            }
        }
        for (int j = 0; sumIdx >= 0x10000 && j < rows; j++) {
            if (((int8_t *)field_0x30)[j] != 0 &&
                ((int8_t *)field_0x34)[j] != 0) {
                signed char lod = ((int8_t *)field_0x2c)[j];
                if (lod < field_0x38 - 1) {
                    AEMesh *prev = (AEMesh *)field_0x24[rows * lod + j];
                    setLod(j, (signed char)(lod + 1));
                    rows = field_0x0;
                    signed char nlod = ((int8_t *)field_0x2c)[j];
                    AEMesh *cur = (AEMesh *)field_0x24[rows * nlod + j];
                    sumIdx = sumIdx - (uint16_t)prev->field_0x28
                                    + (uint16_t)cur->field_0x28;
                }
            }
        }

        int vtxOff = 0;   // r5 (vertex accumulator)
        int idxOff = 0;   // r8 (index accumulator)
        for (int j = 0; j < rows; j++) {
            if (((int8_t *)field_0x30)[j] != 0 &&
                ((int8_t *)field_0x34)[j] != 0) {
                uint8_t *out = (uint8_t *)field_0x20;
                uint8_t mask = out[0];
                signed char lod = ((int8_t *)field_0x2c)[j];
                AEMesh *src = (AEMesh *)field_0x24[rows * lod + j];

                if (mask & 1) {
                    aeabi_memcpy4(*(char **)(out + 4) + vtxOff * 0xc,
                                  src->field_0x4,
                                  (uint32_t)src->field_0x2 * 0xc);
                    out = (uint8_t *)field_0x20;
                    mask = out[0];
                }
                if (mask & 4) {
                    aeabi_memcpy4(*(char **)(out + 0x10) + vtxOff * 0xc,
                                  src->field_0x10,
                                  (uint32_t)src->field_0x2 * 0xc);
                    out = (uint8_t *)field_0x20;
                    mask = out[0];
                }
                if (mask & 8) {
                    aeabi_memcpy4(*(char **)(out + 0xc) + vtxOff * 0x10,
                                  (void *)(uintptr_t)src->field_0xc,
                                  (uint32_t)src->field_0x2 << 4);
                    out = (uint8_t *)field_0x20;
                    mask = out[0];
                }
                if (mask & 2) {
                    aeabi_memcpy4(*(char **)(out + 8) + vtxOff * 8,
                                  src->field_0x8,
                                  (uint32_t)src->field_0x2 << 3);
                    out = (uint8_t *)field_0x20;
                    mask = out[0];
                }
                if (mask & 0x10) {
                    int16_t *si = (int16_t *)src->field_0x2c;
                    int16_t *di = (int16_t *)(*(char **)(out + 0x2c) + idxOff * 2);
                    for (int k = -(int)(uint16_t)src->field_0x28; k != 0; k++) {
                        *di = (int16_t)(*si + (int16_t)vtxOff);
                        si++;
                        di++;
                    }
                }
                rows = field_0x0;
                idxOff += (uint16_t)src->field_0x28;
                vtxOff += (uint16_t)src->field_0x2;
            }
        }
        uint8_t *out = (uint8_t *)field_0x20;
        *(int16_t *)(out + 0x28) = (int16_t)idxOff;
        *(int16_t *)(out + 2) = (int16_t)vtxOff;
        field_0x3c = 0;
    }
}

// ---- LodMeshMerger_181200.cpp ----
extern "C" void *operator_new_array(uint32_t size);              // 0x6ec08
extern "C" void aeabi_memclr4(void *p, uint32_t n);              // 0x6ec14
extern "C" void aeabi_memclr(void *p, uint32_t n);              // 0x6ec20

// LodMeshMerger::LodMeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags)
LodMeshMerger::LodMeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags)
{
    new (&field_0x8) Array<AEMesh*>();
    field_0x20 = 0;
    field_0x24 = 0;
    field_0x28 = 0;
    field_0x2c = 0;
    field_0x14 = canvas;
    field_0x0 = rows;
    field_0x38 = cols;
    field_0x8.resize((uint32_t)(cols * rows));

    uint32_t n = (uint32_t)field_0x0;
    void *slots = operator_new_array(n * cols * 4);
    field_0x24 = (void **)slots;
    aeabi_memclr4(slots, n * cols * 4);

    void *lods = operator_new_array(n | ((int)n >> 31));
    field_0x2c = (int8_t *)lods;
    aeabi_memclr(lods, n);

    char *mats = (char *)operator_new_array(n * 0x3c);
    for (uint32_t off = 0; off != n * 0x3c; off += 0x3c) {
        new ((Matrix *)(mats + off)) Matrix();
    }
    field_0x28 = (Matrix *)mats;

    for (int i = 0; i < (int)field_0x0; i++) {
        Matrix tmp;
        tmp.m[0] = 1.0f;
        tmp.m[1] = 0.0f; tmp.m[2] = 0.0f; tmp.m[3] = 0.0f;
        tmp.m[4] = 1.0f;
        tmp.m[5] = 0.0f; tmp.m[6] = 0.0f; tmp.m[7] = 0.0f; tmp.m[8] = 0.0f;
        tmp.m[9] = 0.0f;
        tmp.m[10] = 1.0f;
        tmp.m[11] = 0.0f; tmp.m[12] = 0.0f; tmp.m[13] = 0.0f;
        tmp.m[14] = 1.0f;
        *(Matrix *)((char *)field_0x28 + i * 0x3c) = tmp;
    }

    uint32_t rn = (uint32_t)field_0x0;
    uint32_t signedN = rn | ((int)rn >> 31);
    char *enabled = (char *)operator_new_array(signedN);
    field_0x30 = (uint8_t *)enabled;
    for (int i = 0; i < (int)rn; i++) enabled[i] = 1;

    char *visible = (char *)operator_new_array(signedN);
    field_0x34 = (uint8_t *)visible;
    for (int i = 0; i < (int)rn; i++) visible[i] = 1;

    field_0x3c = 0;
    field_0x4 = flags;
    field_0x6 = 0;
}

// ---- init_18145a.cpp ----
extern "C" void *LodMeshMerger_transformMesh(void *mesh, const Matrix &m);            // 0x7882c
extern "C" void PaintCanvas_MeshCreate2(void *canvas, uint16_t nv, uint16_t ni, int flags); // 0x75da8
extern "C" void PaintCanvas_TransformCreate(void *canvas, uint32_t *out);             // 0x720ac
extern "C" void PaintCanvas_TransformAddMeshId(void *canvas, uint32_t tf, uint32_t mesh); // 0x73030
extern "C" uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);                            // 0x6ec2c (__aeabi_uidiv)
extern "C" int LodMeshMerger_init_tail(LodMeshMerger *self, int r1, uint16_t flags, uint32_t *meshId); // 0x1ac878

int LodMeshMerger::init()
{
    if (field_0x6 != 0) {
        return field_0x6;
    }

    int rows;
    for (int i = 0; i < (rows = field_0x0); i++) {
        int8_t *lods = field_0x2c;
        int lod = lods[i];
        if (lod >= -1 && field_0x38 <= lod) {
            lods[i] = 0;
        }
        for (int c = 0; c < field_0x38; c++) {
            AEMesh *mesh = field_0x8.data()[field_0x0 * c + i];
            if (mesh != 0) {
                void *t = LodMeshMerger_transformMesh(
                    mesh, *(const Matrix *)((char *)field_0x28 + i * 0x3c));
                field_0x24[field_0x0 * c + i] = t;
            }
        }
    }

    uint16_t nv = 0;
    uint16_t ni = 0;
    for (int i = 0; i < rows; i++) {
        AEMesh *m0 = field_0x8.data()[i];
        uint16_t v = m0->field_0x2;
        uint16_t idiv = aeabi_uidiv16(m0->field_0x28, 3);
        ni = ni + idiv;
        nv = nv + v;
    }

    uint16_t flags = field_0x4;
    PaintCanvas_MeshCreate2(field_0x14, nv, ni,
                            (int)field_0x8.data()[0]->field_0x0);
    void *ptr = PaintCanvas_MeshGetPointer(field_0x14, field_0x18);
    field_0x20 = ptr;
    PaintCanvas_TransformCreate(field_0x14, &field_0x1c);
    PaintCanvas_TransformAddMeshId(field_0x14, field_0x1c, field_0x18);
    field_0x3c = 1;
    return LodMeshMerger_init_tail(this, 0, flags, &field_0x18);
}

// ---- transformMesh_181550.cpp ----
extern "C" {
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
void *LodMeshMerger::transformMesh(AEMesh *src, const Matrix &m)
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

// ---- _LodMeshMerger_181914.cpp ----
extern "C" void operator_delete_array(void *p);  // 0x6ebfc  (operator delete[])
extern "C" void operator_delete(void *p);        // 0x6eb48

// Per-array-slot release thunk read from a hidden PC-relative global (used to
// free the lod/enabled/visible byte arrays at 0x2c/0x30/0x34).
__attribute__((visibility("hidden"))) extern void (*const g_freeFn)(void *);

// Tail-call into the engine base destructor.
extern "C" void LodMeshMerger_base_dtor(void *self);  // 0x1ac888

LodMeshMerger::~LodMeshMerger()
{
    int i = 0;     // element index (r5)
    int idx = 0;   // byte offset (r6)
    void *slots;
    for (;;) {
        slots = field_0x24;
        if (i >= field_0x0 * field_0x38) break;

        void **cell = (void **)((char *)slots + idx);  // slots[idx]
        void **m4 = (void **)((char *)*cell + 4);
        if (*m4 != 0) {
            operator_delete_array(*m4);
            cell = (void **)((char *)field_0x24 + idx);
            m4 = (void **)((char *)*cell + 4);
        }
        *m4 = 0;
        void **m10 = (void **)((char *)*cell + 0x10);
        if (*m10 != 0) {
            operator_delete_array(*m10);
            cell = (void **)((char *)field_0x24 + idx);
            m10 = (void **)((char *)*cell + 0x10);
        }
        *m10 = 0;
        if (*cell != 0) {
            operator_delete(*cell);
            cell = (void **)((char *)field_0x24 + idx);
        }
        *cell = 0;

        idx += 4;
        i += 1;
    }
    if (slots != 0) operator_delete_array(slots);
    field_0x24 = 0;

    g_freeFn(field_0x2c);
    field_0x2c = 0;
    g_freeFn(field_0x30);
    field_0x30 = 0;
    g_freeFn(field_0x34);
    field_0x34 = 0;

    if (field_0x28 != 0) operator_delete_array(field_0x28);
    field_0x28 = 0;
    LodMeshMerger_base_dtor(this);
}

#include "gof2/engine/render/MeshMerger.h"
#include "gof2/engine/render/Mesh.h"
#include "gof2/game/core/Vector.h"
#include "gof2/engine/math/AEMath.h"

// MeshMerger.h forward-declares AbyssEngine::PaintCanvas as a *native* class, which
// conflicts with the `using ::PaintCanvas` re-export inside PaintCanvasClass.h, so we
// cannot include that header here. Instead declare the real global PaintCanvas class
// (whose mangled `PaintCanvas::` symbols match the definitions in PaintCanvas.cpp)
// with exactly the member methods this translation unit calls, and reinterpret_cast
// the engine canvas pointer to it. `PCReal` aliases this complete type so the later
// `#define PaintCanvas AEPaintCanvas` macro does not rewrite the calls.
class PaintCanvas {
public:
    void MeshCreate(unsigned short, unsigned int *, bool);
    void MeshCreate(unsigned short, unsigned short, signed char, unsigned int *);
    void MeshCreate(unsigned short, unsigned short, signed char, unsigned short, unsigned int *);
    void *MeshGetPointer(unsigned int);
    float MeshSetPoint(unsigned int, unsigned short, float, float, float);
    void MeshSetUv(unsigned int, unsigned short, float, float);
    void MeshSetNormal(unsigned int, unsigned short, const AbyssEngine::AEMath::Vector &);
    void MeshSetColor(unsigned int, unsigned short, float, float, float, float);
    void MeshSetTriangle(unsigned int, unsigned short, unsigned short, unsigned short, unsigned short);
    void TransformCreate(unsigned int *);
    void TransformAddMeshId(unsigned int, unsigned int);
    int CameraIsSphereinViewFrustum(void *, float);
};
typedef PaintCanvas PCReal;

// Mesh and PaintCanvas are AbyssEngine types. fwd.h also declares conflicting *global*
// forward decls of the same bare names, so we cannot pull the engine types to global
// scope with `using` (the names already exist). Use the fully-qualified engine names
// throughout this translation unit.
typedef AbyssEngine::Mesh        AEMesh;
typedef AbyssEngine::PaintCanvas AEPaintCanvas;
#define Mesh        AEMesh
#define PaintCanvas AEPaintCanvas

extern "C" uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);

// ---- engine-veneer forwarders ----
// In the binary, init()/render()/setMatrix() each end in a tail-call through a
// PC-relative veneer that performs an indirect call `(*(code*)(DAT+off))(...)` into
// the AbyssEngine PaintCanvas/Matrix entry points. The resolved targets are dynamically
// relocated engine primitives (no static symbol in this .so), NOT inherited base-class
// methods -- proven by MeshMerger::init and LodMeshMerger::init dispatching through
// DISTINCT veneer slots (DAT_001ac6d4 vs DAT_001ac884), which a shared base would not.
// We reproduce that dispatch through the same hidden function-pointer slots so the
// real methods can forward to the engine without an inline indirect-call expression.
//
// Resolved tail targets (android_2.0.16_libgof2hdaa.so):
//   MeshMerger::render   @0x18406c -> (*(DAT_001abda4+0x1abda8))(canvas, transformId, 0)   [g_mmRenderFn]
//   MeshMerger::setMatrix@0x183c74 -> (*(DAT_001abdb4+0x1abdb8))(slot, m)                   [g_mmMatrixAssignFn]
//   MeshMerger::init     @0x183ca6 -> (*(DAT_001ac6d4+0x1ac6d8))(this, r1, flags, &meshId)  [g_mmInitFn]
__attribute__((visibility("hidden"))) extern int  (**g_mmInitFn)(MeshMerger *, int, uint16_t, uint32_t *);
__attribute__((visibility("hidden"))) extern void (**g_mmRenderFn)(void *, void *, int);
__attribute__((visibility("hidden"))) extern void (**g_mmMatrixAssignFn)(void *, const Matrix &);

int MeshMerger::initTail(int r1, uint16_t flags, uint32_t *meshId)
{
    return (*g_mmInitFn)(this, r1, flags, meshId);
}

void MeshMerger::renderTail(void *canvas, void *transformId, int z)
{
    (*g_mmRenderFn)(canvas, transformId, z);
}

void MeshMerger::setMatrixTail(void *matrixSlot, const Matrix &m)
{
    (*g_mmMatrixAssignFn)(matrixSlot, m);
}

// setMatrix(index, m): tail-call the engine matrix-assign with the per-index
// slot (matrices[index], each 0x3c bytes) and the source matrix.

void MeshMerger::setMatrix(int index, const Matrix &m)
{
    void *matricesBase = this->matrices;
    char *matrices = (char *)matricesBase;
    return setMatrixTail(matrices + index * 0x3c, m);
}

void MeshMerger::setLod(int index, signed char lod)
{
    int8_t *lods = (int8_t *)this->lods;
    if (lods[index] != lod) {
        lods[index] = lod;
        if (*((int8_t *)this->enabledFlags + index) == 0) {
            return;
        }
        this->dirty = 1;
    }
}

// render(): tail-call the engine render routine with (canvas, transformId, 0).

void MeshMerger::render()
{
    return renderTail(this->canvas, (void *)(uintptr_t)this->transformId, 0);
}

void MeshMerger::setEnabled(int index, bool enabled)
{
    uint8_t *flags = (uint8_t *)this->enabledFlags;
    if (flags[index] != enabled) {
        flags[index] = enabled;
        if (*((int8_t *)this->visibleFlags + index) == 0) {
            return;
        }
        this->dirty = 1;
    }
}

void MatrixTransformVector(Matrix *out, Vector *v);   // 0x.. transform point
void MatrixRotateVector(Matrix *out, Vector *v);      // 0x.. rotate normal

// MeshMerger::MeshMerger(const Array<uint16_t>& meshIds, Array<Matrix> transforms,
//                        PaintCanvas* canvas, unsigned short flags)
//   Builds a single merged mesh by transforming each source mesh's vertices,
//   normals, uvs, colours and triangles into one combined vertex/index buffer.
MeshMerger::MeshMerger(const Array<uint16_t> &meshIds, Array<Matrix> transforms,
                       PaintCanvas *canvas, uint16_t flags)
{
    this->matrices = 0;
    this->canvas = canvas;
    this->cols = 1;
    this->flags = flags;
    this->rows = (int)transforms.size();

    uint32_t count = meshIds.size();
    uint32_t **table = (uint32_t **)::operator new[](count * 4);
    this->sourceMeshes = table;

    // Per-source meshes: create them and tally vertex/index totals.
    int16_t totalV = 0;
    int16_t totalI = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        uint32_t localId;
        ((PCReal *)canvas)->MeshCreate(meshIds.data()[i], &localId, false);
        void *ptr = ((PCReal *)canvas)->MeshGetPointer(localId);
        ((uint32_t **)this->sourceMeshes)[i] = (uint32_t *)ptr;
        char *m = (char *)((uint32_t **)this->sourceMeshes)[i];
        totalV = (int16_t)(totalV + *(uint16_t *)(m + 2));
        totalI = (int16_t)(totalI + aeabi_uidiv16(*(uint16_t *)(m + 0x28), 3));
    }

    // Create the combined target mesh.
    char *m0 = (char *)((uint32_t **)this->sourceMeshes)[0];
    ((PCReal *)canvas)->MeshCreate((uint16_t)totalV, (uint16_t)totalI, (signed char)*(int8_t *)m0, flags,
                       &this->mergedMeshId);

    int16_t triBase = 0;
    int16_t vtxBase = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        char *m = (char *)((uint32_t **)this->sourceMeshes)[i];
        Matrix *xf = &transforms.data()[i];
        int colOff = 0;
        int uvOff = 0;
        uint16_t nv = *(uint16_t *)(m + 2);
        for (uint16_t v = 0; v < nv; v++) {
            m = (char *)((uint32_t **)this->sourceMeshes)[i];
            uint8_t fl = *(uint8_t *)m;
            Vector tmp;
            if (fl & 1) {
                MatrixTransformVector((Matrix *)&tmp, (Vector *)xf);
                ((PCReal *)canvas)->MeshSetPoint((unsigned int)(long)canvas, (uint16_t)this->mergedMeshId, tmp.x, tmp.y, tmp.z);
                fl = *(uint8_t *)m;
            }
            if (((uint32_t)fl << 0x1d) & 0x80000000u) {
                MatrixRotateVector((Matrix *)&tmp, (Vector *)xf);
                ((PCReal *)canvas)->MeshSetNormal(this->mergedMeshId, (int16_t)(vtxBase + v), tmp);
                fl = *(uint8_t *)m;
            }
            if (((uint32_t)fl << 0x1e) & 0x80000000u) {
                float *uv = (float *)(*(int *)(m + 8) + uvOff);
                ((PCReal *)canvas)->MeshSetUv((unsigned int)(long)canvas, (uint16_t)this->mergedMeshId, uv[1], uv[0]);
                fl = *(uint8_t *)m;
            }
            if (((uint32_t)fl << 0x1c) & 0x80000000u) {
                float *col = (float *)(*(int *)(m + 0xc) + colOff);
                ((PCReal *)canvas)->MeshSetColor(this->mergedMeshId, (int16_t)(vtxBase + v),
                                     col[1], col[0], col[2], col[3]);
            }
            uvOff += 8;
            colOff += 0x10;
        }

        uint16_t tris = aeabi_uidiv16(*(uint16_t *)(m + 0x28), 3);
        int triOff = 0;
        for (uint16_t t = 0; t < tris; t++) {
            if (((uint32_t)(*(uint8_t *)m) << 0x1b) & 0x80000000u) {
                int16_t *ix = (int16_t *)(*(int *)(m + 0x2c) + triOff);
                ((PCReal *)canvas)->MeshSetTriangle((unsigned int)(long)canvas, (uint16_t)this->mergedMeshId,
                                        (int16_t)(triBase + t),
                                        (int16_t)(ix[0] + vtxBase), (int16_t)(ix[1] + vtxBase));
                m = (char *)((uint32_t **)this->sourceMeshes)[i];
            }
            triOff += 6;
        }
        triBase = (int16_t)(triBase + aeabi_uidiv16(*(uint16_t *)(m + 0x28), 3));
        vtxBase = (int16_t)(vtxBase + *(uint16_t *)(m + 2));
    }

    ((PCReal *)canvas)->TransformCreate(&this->transformId);
    ((PCReal *)canvas)->TransformAddMeshId(this->transformId, this->mergedMeshId);
    this->transformedMeshes = 0;
    this->initialized = 1;
    this->mergedMesh = 0;
    this->lods = 0;
    this->dirty = 0;
}

extern "C" void aeabi_memcpy4(void *dst, const void *src, uint32_t n);  // __aeabi_memcpy4

void MeshMerger::update()
{
    int rows = this->rows;
    for (int i = 0; i < rows; i++) {
        void *sph = ((void **)this->transformedMeshes)[i];
        uint8_t vis = (uint8_t)((PCReal *)this->canvas)->CameraIsSphereinViewFrustum(
            (void *)((char *)sph + 0x3c),
            *(float *)((char *)sph + 0x48));
        int8_t *visArr = (int8_t *)this->visibleFlags;
        if (vis != (uint8_t)visArr[i]) {
            visArr[i] = (int8_t)vis;
            if (*((int8_t *)this->enabledFlags + i) != 0) {
                this->dirty = 1;
            }
        }
        rows = this->rows;
    }

    if (this->dirty != 0) {
        int idxOff = 0;   // iVar9 (index accumulator)
        int vtxOff = 0;   // iVar7 (vertex accumulator)
        for (int j = 0; j < rows; j++) {
            if (*((int8_t *)this->enabledFlags + j) != 0 &&
                *((int8_t *)this->visibleFlags + j) != 0) {
                uint8_t *out = (uint8_t *)this->mergedMesh;
                uint8_t mask = out[0];
                signed char lod = *((int8_t *)this->lods + j);
                Mesh *src = (Mesh *)((void **)this->transformedMeshes)[rows * lod + j];

                if (mask & 1) {
                    aeabi_memcpy4(*(char **)(out + 4) + vtxOff * 0xc,
                                  src->field_0x4,
                                  (uint32_t)src->field_0x2 * 0xc);
                    out = (uint8_t *)this->mergedMesh;
                    mask = out[0];
                }
                if (mask & 4) {
                    aeabi_memcpy4(*(char **)(out + 0x10) + vtxOff * 0xc,
                                  src->field_0x10,
                                  (uint32_t)src->field_0x2 * 0xc);
                    out = (uint8_t *)this->mergedMesh;
                    mask = out[0];
                }
                if (mask & 8) {
                    aeabi_memcpy4(*(char **)(out + 0xc) + vtxOff * 0x10,
                                  (const void *)(uintptr_t)src->field_0xc,
                                  (uint32_t)src->field_0x2 << 4);
                    out = (uint8_t *)this->mergedMesh;
                    mask = out[0];
                }
                if (mask & 2) {
                    aeabi_memcpy4(*(char **)(out + 8) + vtxOff * 8,
                                  src->field_0x8,
                                  (uint32_t)src->field_0x2 << 3);
                    out = (uint8_t *)this->mergedMesh;
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
                rows = this->rows;
                idxOff += (uint16_t)src->field_0x28;
                vtxOff += (uint16_t)src->field_0x2;
            }
        }
        uint8_t *out = (uint8_t *)this->mergedMesh;
        *(int16_t *)(out + 0x28) = (int16_t)idxOff;
        *(int16_t *)(out + 2) = (int16_t)vtxOff;
        this->dirty = 0;
    }
}

extern "C" uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);                // 0x6ec2c (__aeabi_uidiv)

int MeshMerger::init()
{
    if (this->initialized != 0) {
        return this->initialized;
    }

    void *matricesBase = this->matrices;
    int rows;
    for (int i = 0; i < (rows = this->rows); i++) {
        int8_t *lods = (int8_t *)this->lods;
        int lod = lods[i];
        if (lod >= -1 && this->cols <= lod) {
            lods[i] = 0;
        }
        for (int c = 0; c < this->cols; c++) {
            void *mesh = ((void **)this->sourceMeshes)[this->rows * c + i];
            if (mesh != 0) {
                char *matrices = (char *)matricesBase;
                void *t = this->transformMesh((Mesh *)mesh, *(const Matrix *)(matrices + i * 0x3c));
                ((void **)this->transformedMeshes)[this->rows * c + i] = t;
            }
        }
    }

    uint16_t nv = 0;
    uint16_t ni = 0;
    for (int i = 0; i < rows; i++) {
        void *m0 = ((void **)this->sourceMeshes)[i];
        uint16_t v = *(uint16_t *)((char *)m0 + 2);
        uint16_t idiv = aeabi_uidiv16(*(uint16_t *)((char *)m0 + 0x28), 3);
        ni = ni + idiv;
        nv = nv + v;
    }

    uint16_t flags = this->flags;
    PaintCanvas *canvas = (PaintCanvas *)this->canvas;
    ((PCReal *)canvas)->MeshCreate(nv, ni, (signed char)*(int8_t *)*(void **)&this->sourceMeshes,
                       &this->mergedMeshId);
    void *ptr = ((PCReal *)canvas)->MeshGetPointer(this->mergedMeshId);
    this->mergedMesh = ptr;   // merged-mesh pointer (full-width store)
    ((PCReal *)canvas)->TransformCreate(&this->transformId);
    ((PCReal *)canvas)->TransformAddMeshId(this->transformId, this->mergedMeshId);
    this->dirty = 1;
    return initTail(0, flags, &this->mergedMeshId);
}

extern "C" void *aeabi_memclr4(void *p, uint32_t n);
extern "C" void *aeabi_memclr(void *p, uint32_t n);
// Raw allocator thunk fetched from a hidden global (DAT_00183bf4).
__attribute__((visibility("hidden"))) extern void *(**g_allocFn)(int);

// MeshMerger::MeshMerger(int rows, int cols, PaintCanvas*, unsigned short flags)
//   Allocates the per-cell mesh/transformed-mesh tables, the per-row LOD/matrix
//   arrays (matrices seeded to identity), and the enabled/visible byte arrays.
MeshMerger::MeshMerger(int rows, int cols, PaintCanvas *canvas, uint16_t flags)
{
    void *(*alloc)(int) = *g_allocFn;

    // The binary zero-initializes the 0x18..0x24 region as four 0.0f stores
    // (a transient scratch blob whose bit pattern is plain zero); the slots are
    // overwritten with real allocations below.
    this->transformedMeshes = 0;
    this->matrices = 0;
    this->mergedMesh = 0;
    this->lods = 0;
    this->canvas = canvas;
    this->rows = rows;
    this->cols = cols;

    int bytes = cols * rows * 4;
    long long need = (long long)(unsigned)(cols * rows) * 4;
    int req = (int)((need >> 32) != 0 ? 0xffffffff : (unsigned)need);

    this->sourceMeshes = alloc(req);
    aeabi_memclr4(this->sourceMeshes, bytes);
    this->transformedMeshes = alloc(req);
    aeabi_memclr4(this->transformedMeshes, bytes);

    this->lods = alloc(rows | (rows >> 31));
    aeabi_memclr(this->lods, rows);

    long long mneed = (long long)(unsigned)rows * 0x3c;
    int mreq = (int)((mneed >> 32) != 0 ? 0xffffffff : (unsigned)mneed);
    char *matrices = (char *)alloc(mreq);
    for (int off = 0; rows != 0 && off != rows * 0x3c; off += 0x3c)
        new ((void *)(matrices + off)) Matrix();
    this->matrices = matrices;

    Matrix ident;   // engine identity matrix
    int n = this->rows;
    char *mp = matrices;
    for (int i = 0, off = 0; i < n; i++, off += 0x3c)
        *(Matrix *)(mp + off) = ident;

    n = this->rows;
    uint8_t *en = (uint8_t *)::operator new[](n | (n >> 31));
    this->enabledFlags = en;
    for (int i = 0; i < n; i++) en[i] = 1;

    uint8_t *vis = (uint8_t *)::operator new[](n | (n >> 31));
    this->visibleFlags = vis;
    for (int i = 0; i < n; i++) vis[i] = 1;

    this->dirty = 0;
    this->flags = flags;
    this->initialized = 0;
}

void MeshMerger::setMesh(int index, signed char lod, uint16_t meshId)
{
    uint32_t id;
    PaintCanvas *canvas = (PaintCanvas *)this->canvas;
    ((PCReal *)canvas)->MeshCreate(meshId, &id, false);
    void *ptr = ((PCReal *)canvas)->MeshGetPointer(id);
    void **meshes = (void **)this->sourceMeshes;
    meshes[this->rows * lod + index] = ptr;
}

extern "C" void AEMath_MatrixTransformVector(Vector *out, const Vector *v);
extern "C" void AEMath_MatrixRotateVector(Vector *out, const Vector *v);
extern "C" void AEMath_BSphere_assign(void *dst, const void *src);

void *MeshMerger::transformMesh(Mesh *mesh, const Matrix &m)
{
    char *out = (char *)::operator new(0x88);
    // Zero/identity initialize the new Mesh header.
    *(uint32_t *)(out + 0x82) = 0;
    *(uint32_t *)(out + 0x7e) = 0;
    out[0x38] = 0;
    *(uint32_t *)(out + 0x4c) = 0x3f800000;
    *(uint32_t *)(out + 0x50) = 0;
    *(uint32_t *)(out + 0x54) = 0;
    *(uint32_t *)(out + 0x59) = 0;
    __builtin_memset(out + 0x70, 0, 16);
    __builtin_memset(out + 0x60, 0, 16);
    __builtin_memset(out + 0x28, 0, 16);
    __builtin_memset(out + 0x14, 0, 16);
    __builtin_memset(out + 0x4, 0, 16);
    __builtin_memset(out + 0x3c, 0, 16);
    *(uint32_t *)(out + 0x55) = 0;

    uint16_t nv = mesh->field_0x2;
    *(uint16_t *)(out + 2) = nv;
    *(uint16_t *)(out + 0x28) = mesh->field_0x28;
    uint8_t flags = *(uint8_t *)mesh;
    *(uint8_t *)out = flags;

    if (flags & 2) {
        *(void **)(out + 8) = mesh->field_0x8;       // pointer field (full-width store)
    }
    if (flags & 8) {
        *(uint32_t *)(out + 0xc) = mesh->field_0xc;  // field_0xc is uint32_t
    }
    if (flags & 0x10) {
        *(void **)(out + 0x2c) = mesh->field_0x2c;   // pointer field (full-width store)
    }
    if (flags & 1) {
        void *buf = ::operator new[]((uint32_t)nv * 0xc);
        *(void **)(out + 4) = buf;
        int off = 0;
        for (uint32_t k = 0; k < nv; k++) {
            Vector tmp;
            AEMath_MatrixTransformVector(&tmp, (const Vector *)&m);
            *(Vector *)(*(char **)(out + 4) + off) = tmp;
            off += 0xc;
            nv = mesh->field_0x2;
        }
        flags = *(uint8_t *)mesh;
    }
    if (flags & 4) {
        void *buf = ::operator new[]((uint32_t)nv * 0xc);
        *(void **)(out + 0x10) = buf;
        int off = 0;
        for (uint32_t k = 0; k < nv; k++) {
            Vector rot;
            AEMath_MatrixRotateVector(&rot, (const Vector *)&m);
            Vector nrm = AbyssEngine::AEMath::VectorNormalize(rot);
            *(Vector *)(*(char **)(out + 0x10) + off) = nrm;
            off += 0xc;
            nv = mesh->field_0x2;
        }
    }

    // Build the bounding sphere from the transformed center + original radius.
    Vector center;
    AEMath_MatrixTransformVector(&center, (const Vector *)&m);
    float bs[4];
    bs[0] = center.x;
    bs[1] = center.y;
    bs[2] = center.z;
    bs[3] = 1.0f;
    *(uint32_t *)&bs[2] = mesh->field_0x48;  // radius overrides z slot? matches uStack_44
    AEMath_BSphere_assign(out + 0x3c, bs);
    return out;
}

// Per-array-slot release thunk fetched from a hidden global (called for the
// lod/enabled/visible byte arrays at 0x24/0x28/0x2c).
__attribute__((visibility("hidden"))) extern void (**g_freeFn)(void *);

MeshMerger::~MeshMerger()
{
    if (this->sourceMeshes != 0) ::operator delete[](this->sourceMeshes);
    this->sourceMeshes = 0;

    int i = 0;         // element index (iVar4)
    int idx = 0;       // byte offset (iVar5)
    void *slots;
    for (;;) {
        slots = this->transformedMeshes;
        if (i >= this->rows * this->cols) break;

        void **cell = (void **)((char *)slots + idx);  // slots[i]
        void **m4 = (void **)((char *)*cell + 4);
        if (*m4 != 0) {
            ::operator delete[](*m4);
            cell = (void **)((char *)slots + idx);
            m4 = (void **)((char *)*cell + 4);
        }
        *m4 = 0;
        void **m10 = (void **)((char *)*cell + 0x10);
        if (*m10 != 0) {
            ::operator delete[](*m10);
            cell = (void **)((char *)slots + idx);
            m10 = (void **)((char *)*cell + 0x10);
        }
        *m10 = 0;
        if (*cell != 0) {
            ::operator delete(*cell);
            cell = (void **)((char *)slots + idx);
        }
        *cell = 0;

        idx += 4;
        i += 1;
    }
    if (slots != 0) ::operator delete[](slots);
    this->transformedMeshes = 0;

    void (*freeFn)(void *) = *g_freeFn;
    freeFn(this->lods);
    this->lods = 0;
    freeFn(this->enabledFlags);
    this->enabledFlags = 0;
    freeFn(this->visibleFlags);
    this->visibleFlags = 0;

    if (this->matrices != 0) ::operator delete[](this->matrices);
    this->matrices = 0;
}

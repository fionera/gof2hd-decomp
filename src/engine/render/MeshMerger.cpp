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

extern "C" void MeshMerger_setMatrix_tail(void *dst, const Matrix &m);
extern "C" void MeshMerger_render_tail(void *a, void *b, int z);
extern "C" uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);

// ---- engine-veneer forwarders ----
// In the binary, init()/render()/setMatrix() each end in a tail-call through a
// PC-relative GOT/veneer slot into the AbyssEngine PaintCanvas/Matrix entry points.
// We reproduce that dispatch through the same hidden function-pointer slots so the
// real methods can forward to the engine without an inline indirect-call expression.
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
    char *base = (char *)pp(this, 0x1c);
    return MeshMerger_setMatrix_tail(base + index * 0x3c, m);
}

void MeshMerger::setLod(int index, signed char lod)
{
    int8_t *lods = (int8_t *)pp(this, 0x24);
    if (lods[index] != lod) {
        lods[index] = lod;
        if (*((int8_t *)pp(this, 0x28) + index) == 0) {
            return;
        }
        u8(this, 0x34) = 1;
    }
}

// render(): tail-call the engine render routine with (field_0xc, field_0x14, 0).

void MeshMerger::render()
{
    return MeshMerger_render_tail(pp(this, 0xc), pp(this, 0x14), 0);
}

void MeshMerger::setEnabled(int index, bool enabled)
{
    uint8_t *flags = (uint8_t *)pp(this, 0x28);
    if (flags[index] != enabled) {
        flags[index] = enabled;
        if (*((int8_t *)pp(this, 0x2c) + index) == 0) {
            return;
        }
        u8(this, 0x34) = 1;
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
    pp(this, 0x1c) = 0;
    pp(this, 0xc) = canvas;
    i32(this, 0x30) = 1;
    u16(this, 0x4) = flags;
    i32(this, 0x00) = (int)transforms.size();

    uint32_t count = meshIds.size();
    uint32_t **table = (uint32_t **)::operator new[](count * 4);
    pp(this, 0x8) = table;

    // Per-source meshes: create them and tally vertex/index totals.
    int16_t totalV = 0;
    int16_t totalI = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        uint32_t localId;
        ((PCReal *)canvas)->MeshCreate(meshIds.data()[i], &localId, false);
        void *ptr = ((PCReal *)canvas)->MeshGetPointer(localId);
        ((uint32_t **)pp(this, 0x8))[i] = (uint32_t *)ptr;
        char *m = (char *)((uint32_t **)pp(this, 0x8))[i];
        totalV = (int16_t)(totalV + *(uint16_t *)(m + 2));
        totalI = (int16_t)(totalI + aeabi_uidiv16(*(uint16_t *)(m + 0x28), 3));
    }

    // Create the combined target mesh.
    char *m0 = (char *)((uint32_t **)pp(this, 0x8))[0];
    ((PCReal *)canvas)->MeshCreate((uint16_t)totalV, (uint16_t)totalI, (signed char)*(int8_t *)m0, flags,
                       (uint32_t *)((char *)this + 0x10));

    int16_t triBase = 0;
    int16_t vtxBase = 0;
    for (uint32_t i = 0; i < meshIds.size(); i++) {
        char *m = (char *)((uint32_t **)pp(this, 0x8))[i];
        Matrix *xf = &transforms.data()[i];
        int colOff = 0;
        int uvOff = 0;
        uint16_t nv = *(uint16_t *)(m + 2);
        for (uint16_t v = 0; v < nv; v++) {
            m = (char *)((uint32_t **)pp(this, 0x8))[i];
            uint8_t fl = *(uint8_t *)m;
            Vector tmp;
            if (fl & 1) {
                MatrixTransformVector((Matrix *)&tmp, (Vector *)xf);
                ((PCReal *)canvas)->MeshSetPoint((unsigned int)(long)canvas, (uint16_t)u32(this, 0x10), tmp.x, tmp.y, tmp.z);
                fl = *(uint8_t *)m;
            }
            if (((uint32_t)fl << 0x1d) & 0x80000000u) {
                MatrixRotateVector((Matrix *)&tmp, (Vector *)xf);
                ((PCReal *)canvas)->MeshSetNormal(u32(this, 0x10), (int16_t)(vtxBase + v), tmp);
                fl = *(uint8_t *)m;
            }
            if (((uint32_t)fl << 0x1e) & 0x80000000u) {
                float *uv = (float *)(*(int *)(m + 8) + uvOff);
                ((PCReal *)canvas)->MeshSetUv((unsigned int)(long)canvas, (uint16_t)u32(this, 0x10), uv[1], uv[0]);
                fl = *(uint8_t *)m;
            }
            if (((uint32_t)fl << 0x1c) & 0x80000000u) {
                float *col = (float *)(*(int *)(m + 0xc) + colOff);
                ((PCReal *)canvas)->MeshSetColor(u32(this, 0x10), (int16_t)(vtxBase + v),
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
                ((PCReal *)canvas)->MeshSetTriangle((unsigned int)(long)canvas, (uint16_t)u32(this, 0x10),
                                        (int16_t)(triBase + t),
                                        (int16_t)(ix[0] + vtxBase), (int16_t)(ix[1] + vtxBase));
                m = (char *)((uint32_t **)pp(this, 0x8))[i];
            }
            triOff += 6;
        }
        triBase = (int16_t)(triBase + aeabi_uidiv16(*(uint16_t *)(m + 0x28), 3));
        vtxBase = (int16_t)(vtxBase + *(uint16_t *)(m + 2));
    }

    ((PCReal *)canvas)->TransformCreate((uint32_t *)((char *)this + 0x14));
    ((PCReal *)canvas)->TransformAddMeshId(u32(this, 0x14), u32(this, 0x10));
    pp(this, 0x18) = 0;
    u8(this, 0x6) = 1;
    i32(this, 0x20) = 0;
    i32(this, 0x24) = 0;
    u8(this, 0x34) = 0;
}

extern "C" void aeabi_memcpy4(void *dst, const void *src, uint32_t n);  // __aeabi_memcpy4

void MeshMerger::update()
{
    int rows = i32(this, 0x0);
    for (int i = 0; i < rows; i++) {
        void *sph = ((void **)pp(this, 0x18))[i];
        uint8_t vis = (uint8_t)((PCReal *)pp(this, 0xc))->CameraIsSphereinViewFrustum(
            (void *)((char *)sph + 0x3c),
            *(float *)((char *)sph + 0x48));
        int8_t *visArr = (int8_t *)pp(this, 0x2c);
        if (vis != (uint8_t)visArr[i]) {
            visArr[i] = (int8_t)vis;
            if (*((int8_t *)pp(this, 0x28) + i) != 0) {
                u8(this, 0x34) = 1;
            }
        }
        rows = i32(this, 0x0);
    }

    if (u8(this, 0x34) != 0) {
        int idxOff = 0;   // iVar9 (index accumulator)
        int vtxOff = 0;   // iVar7 (vertex accumulator)
        for (int j = 0; j < rows; j++) {
            if (*((int8_t *)pp(this, 0x28) + j) != 0 &&
                *((int8_t *)pp(this, 0x2c) + j) != 0) {
                uint8_t *out = (uint8_t *)pp(this, 0x20);
                uint8_t mask = out[0];
                signed char lod = *((int8_t *)pp(this, 0x24) + j);
                Mesh *src = (Mesh *)((void **)pp(this, 0x18))[rows * lod + j];

                if (mask & 1) {
                    aeabi_memcpy4(*(char **)(out + 4) + vtxOff * 0xc,
                                  src->field_0x4,
                                  (uint32_t)src->field_0x2 * 0xc);
                    out = (uint8_t *)pp(this, 0x20);
                    mask = out[0];
                }
                if (mask & 4) {
                    aeabi_memcpy4(*(char **)(out + 0x10) + vtxOff * 0xc,
                                  src->field_0x10,
                                  (uint32_t)src->field_0x2 * 0xc);
                    out = (uint8_t *)pp(this, 0x20);
                    mask = out[0];
                }
                if (mask & 8) {
                    aeabi_memcpy4(*(char **)(out + 0xc) + vtxOff * 0x10,
                                  (const void *)(uintptr_t)src->field_0xc,
                                  (uint32_t)src->field_0x2 << 4);
                    out = (uint8_t *)pp(this, 0x20);
                    mask = out[0];
                }
                if (mask & 2) {
                    aeabi_memcpy4(*(char **)(out + 8) + vtxOff * 8,
                                  src->field_0x8,
                                  (uint32_t)src->field_0x2 << 3);
                    out = (uint8_t *)pp(this, 0x20);
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
                rows = i32(this, 0x0);
                idxOff += (uint16_t)src->field_0x28;
                vtxOff += (uint16_t)src->field_0x2;
            }
        }
        uint8_t *out = (uint8_t *)pp(this, 0x20);
        *(int16_t *)(out + 0x28) = (int16_t)idxOff;
        *(int16_t *)(out + 2) = (int16_t)vtxOff;
        u8(this, 0x34) = 0;
    }
}

extern "C" uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);                // 0x6ec2c (__aeabi_uidiv)
extern "C" int MeshMerger_init_tail(MeshMerger *self, int r1, uint16_t flags, uint32_t *meshId);

int MeshMerger::init()
{
    if (u8(this, 0x6) != 0) {
        return u8(this, 0x6);
    }

    int rows;
    for (int i = 0; i < (rows = i32(this, 0x0)); i++) {
        int8_t *lods = (int8_t *)pp(this, 0x24);
        int lod = lods[i];
        if (lod >= -1 && i32(this, 0x30) <= lod) {
            lods[i] = 0;
        }
        for (int c = 0; c < i32(this, 0x30); c++) {
            void *mesh = ((void **)pp(this, 0x8))[i32(this, 0x0) * c + i];
            if (mesh != 0) {
                void *t = this->transformMesh((Mesh *)mesh, *(const Matrix *)((char *)pp(this, 0x1c) + i * 0x3c));
                ((void **)pp(this, 0x18))[i32(this, 0x0) * c + i] = t;
            }
        }
    }

    uint16_t nv = 0;
    uint16_t ni = 0;
    for (int i = 0; i < rows; i++) {
        void *m0 = ((void **)pp(this, 0x8))[i];
        uint16_t v = *(uint16_t *)((char *)m0 + 2);
        uint16_t idiv = aeabi_uidiv16(*(uint16_t *)((char *)m0 + 0x28), 3);
        ni = ni + idiv;
        nv = nv + v;
    }

    uint16_t flags = u16(this, 0x4);
    PaintCanvas *canvas = (PaintCanvas *)pp(this, 0xc);
    ((PCReal *)canvas)->MeshCreate(nv, ni, (signed char)*(int8_t *)*(void **)pp(this, 0x8),
                       (uint32_t *)((char *)this + 0x10));
    void *ptr = ((PCReal *)canvas)->MeshGetPointer(u32(this, 0x10));
    pp(this, 0x20) = ptr;   // field_0x20: merged-mesh pointer (full-width store)
    ((PCReal *)canvas)->TransformCreate((uint32_t *)((char *)this + 0x14));
    ((PCReal *)canvas)->TransformAddMeshId(u32(this, 0x14), u32(this, 0x10));
    u8(this, 0x34) = 1;
    return MeshMerger_init_tail(this, 0, flags, (uint32_t *)((char *)this + 0x10));
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

    f32(this, 0x18) = 0.0f;
    f32(this, 0x1c) = 0.0f;
    f32(this, 0x20) = 0.0f;
    f32(this, 0x24) = 0.0f;
    pp(this, 0xc) = canvas;
    i32(this, 0x00) = rows;
    i32(this, 0x30) = cols;

    int bytes = cols * rows * 4;
    long long need = (long long)(unsigned)(cols * rows) * 4;
    int req = (int)((need >> 32) != 0 ? 0xffffffff : (unsigned)need);

    pp(this, 0x8) = alloc(req);
    aeabi_memclr4(pp(this, 0x8), bytes);
    pp(this, 0x18) = alloc(req);
    aeabi_memclr4(pp(this, 0x18), bytes);

    pp(this, 0x24) = alloc(rows | (rows >> 31));
    aeabi_memclr(pp(this, 0x24), rows);

    long long mneed = (long long)(unsigned)rows * 0x3c;
    int mreq = (int)((mneed >> 32) != 0 ? 0xffffffff : (unsigned)mneed);
    char *matrices = (char *)alloc(mreq);
    for (int off = 0; rows != 0 && off != rows * 0x3c; off += 0x3c)
        new ((void *)(matrices + off)) Matrix();
    pp(this, 0x1c) = matrices;

    Matrix ident;   // engine identity matrix
    int n = i32(this, 0x00);
    char *mp = (char *)pp(this, 0x1c);
    for (int i = 0, off = 0; i < n; i++, off += 0x3c)
        *(Matrix *)(mp + off) = ident;

    n = i32(this, 0x00);
    uint8_t *en = (uint8_t *)::operator new[](n | (n >> 31));
    pp(this, 0x28) = en;
    for (int i = 0; i < n; i++) en[i] = 1;

    uint8_t *vis = (uint8_t *)::operator new[](n | (n >> 31));
    pp(this, 0x2c) = vis;
    for (int i = 0; i < n; i++) vis[i] = 1;

    u8(this, 0x34) = 0;
    u16(this, 0x4) = flags;
    u8(this, 0x6) = 0;
}

void MeshMerger::setMesh(int index, signed char lod, uint16_t meshId)
{
    uint32_t id;
    PaintCanvas *canvas = (PaintCanvas *)pp(this, 0xc);
    ((PCReal *)canvas)->MeshCreate(meshId, &id, false);
    void *ptr = ((PCReal *)canvas)->MeshGetPointer(id);
    void **meshes = (void **)pp(this, 0x8);
    meshes[i32(this, 0x0) * lod + index] = ptr;
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
    if (pp(this, 0x8) != 0) ::operator delete[](pp(this, 0x8));
    pp(this, 0x8) = 0;

    int i = 0;         // element index (iVar4)
    int idx = 0;       // byte offset (iVar5)
    void *slots;
    for (;;) {
        slots = pp(this, 0x18);
        if (i >= i32(this, 0x0) * i32(this, 0x30)) break;

        void **cell = (void **)((char *)slots + idx);  // slots[i]
        void **m4 = (void **)((char *)*cell + 4);
        if (*m4 != 0) {
            ::operator delete[](*m4);
            cell = (void **)((char *)pp(this, 0x18) + idx);
            m4 = (void **)((char *)*cell + 4);
        }
        *m4 = 0;
        void **m10 = (void **)((char *)*cell + 0x10);
        if (*m10 != 0) {
            ::operator delete[](*m10);
            cell = (void **)((char *)pp(this, 0x18) + idx);
            m10 = (void **)((char *)*cell + 0x10);
        }
        *m10 = 0;
        if (*cell != 0) {
            ::operator delete(*cell);
            cell = (void **)((char *)pp(this, 0x18) + idx);
        }
        *cell = 0;

        idx += 4;
        i += 1;
    }
    if (slots != 0) ::operator delete[](slots);
    pp(this, 0x18) = 0;

    void (*freeFn)(void *) = *g_freeFn;
    freeFn(pp(this, 0x24));
    pp(this, 0x24) = 0;
    freeFn(pp(this, 0x28));
    pp(this, 0x28) = 0;
    freeFn(pp(this, 0x2c));
    pp(this, 0x2c) = 0;

    if (pp(this, 0x1c) != 0) ::operator delete[](pp(this, 0x1c));
    pp(this, 0x1c) = 0;
}

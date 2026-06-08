#include "class.h"

// PaintCanvas mesh-building entry points (resolved blx targets).
extern "C" void PaintCanvas_MeshCreate_simple(PaintCanvas *c, uint16_t nv, uint32_t *outId, bool b); // 0x75d.. (MeshCreate 4-arg)
extern "C" void PaintCanvas_MeshCreate_full(PaintCanvas *c, int16_t nv, int16_t ni, int flags, uint16_t f, void *outId); // MeshCreate 6-arg
extern "C" uint32_t PaintCanvas_MeshGetPointer(PaintCanvas *c, uint32_t id);
extern "C" void PaintCanvas_MeshSetPoint(PaintCanvas *c, uint16_t mesh, float x, float y, float z);
extern "C" void PaintCanvas_MeshSetNormal(PaintCanvas *c, uint32_t mesh, int16_t idx, Vector *n);
extern "C" void PaintCanvas_MeshSetUv(PaintCanvas *c, uint16_t mesh, float u, float v);
extern "C" void PaintCanvas_MeshSetColor(PaintCanvas *c, uint32_t mesh, int16_t idx, float r, float g, float b, float a);
extern "C" void PaintCanvas_MeshSetTriangle(PaintCanvas *c, uint16_t mesh, int16_t tri, int16_t a, int16_t b);
extern "C" void PaintCanvas_TransformCreate(PaintCanvas *c, uint32_t *out);
extern "C" void PaintCanvas_TransformAddMeshId(PaintCanvas *c, uint32_t tf, uint32_t mesh);

extern "C" void MatrixTransformVector(Matrix *out, Vector *v);   // 0x.. transform point
extern "C" void MatrixRotateVector(Matrix *out, Vector *v);      // 0x.. rotate normal
extern "C" void *operator_new_array(uint32_t n);
extern "C" uint16_t aeabi_uidiv16(uint16_t a, uint16_t b);

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
    i32(this, 0x00) = (int)transforms.size;

    uint32_t count = meshIds.size;
    uint32_t **table = (uint32_t **)operator_new_array(count * 4);
    pp(this, 0x8) = table;

    // Per-source meshes: create them and tally vertex/index totals.
    int16_t totalV = 0;
    int16_t totalI = 0;
    for (uint32_t i = 0; i < meshIds.size; i++) {
        uint32_t localId;
        PaintCanvas_MeshCreate_simple(canvas, meshIds.data[i], &localId, false);
        uint32_t ptr = PaintCanvas_MeshGetPointer(canvas, localId);
        ((uint32_t **)pp(this, 0x8))[i] = (uint32_t *)ptr;
        char *m = (char *)((uint32_t **)pp(this, 0x8))[i];
        totalV = (int16_t)(totalV + *(uint16_t *)(m + 2));
        totalI = (int16_t)(totalI + aeabi_uidiv16(*(uint16_t *)(m + 0x28), 3));
    }

    // Create the combined target mesh.
    char *m0 = (char *)((uint32_t **)pp(this, 0x8))[0];
    PaintCanvas_MeshCreate_full(canvas, totalV, totalI, (int)*(int8_t *)m0, flags,
                                (void *)((char *)this + 0x10));

    int16_t triBase = 0;
    int16_t vtxBase = 0;
    for (uint32_t i = 0; i < meshIds.size; i++) {
        char *m = (char *)((uint32_t **)pp(this, 0x8))[i];
        Matrix *xf = &transforms.data[i];
        int colOff = 0;
        int uvOff = 0;
        uint16_t nv = *(uint16_t *)(m + 2);
        for (uint16_t v = 0; v < nv; v++) {
            m = (char *)((uint32_t **)pp(this, 0x8))[i];
            uint8_t fl = *(uint8_t *)m;
            Vector tmp;
            if (fl & 1) {
                MatrixTransformVector((Matrix *)&tmp, (Vector *)xf);
                PaintCanvas_MeshSetPoint(canvas, (uint16_t)u32(this, 0x10), tmp.x, tmp.y, tmp.z);
                fl = *(uint8_t *)m;
            }
            if (((uint32_t)fl << 0x1d) & 0x80000000u) {
                MatrixRotateVector((Matrix *)&tmp, (Vector *)xf);
                PaintCanvas_MeshSetNormal(canvas, u32(this, 0x10), (int16_t)(vtxBase + v), &tmp);
                fl = *(uint8_t *)m;
            }
            if (((uint32_t)fl << 0x1e) & 0x80000000u) {
                float *uv = (float *)(*(int *)(m + 8) + uvOff);
                PaintCanvas_MeshSetUv(canvas, (uint16_t)u32(this, 0x10), uv[1], uv[0]);
                fl = *(uint8_t *)m;
            }
            if (((uint32_t)fl << 0x1c) & 0x80000000u) {
                float *col = (float *)(*(int *)(m + 0xc) + colOff);
                PaintCanvas_MeshSetColor(canvas, u32(this, 0x10), (int16_t)(vtxBase + v),
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
                PaintCanvas_MeshSetTriangle(canvas, (uint16_t)u32(this, 0x10),
                                            (int16_t)(triBase + t),
                                            (int16_t)(ix[0] + vtxBase), (int16_t)(ix[1] + vtxBase));
                m = (char *)((uint32_t **)pp(this, 0x8))[i];
            }
            triOff += 6;
        }
        triBase = (int16_t)(triBase + aeabi_uidiv16(*(uint16_t *)(m + 0x28), 3));
        vtxBase = (int16_t)(vtxBase + *(uint16_t *)(m + 2));
    }

    PaintCanvas_TransformCreate(canvas, (uint32_t *)((char *)this + 0x14));
    PaintCanvas_TransformAddMeshId(canvas, u32(this, 0x14), u32(this, 0x10));
    pp(this, 0x18) = 0;
    u8(this, 0x6) = 1;
    i32(this, 0x20) = 0;
    i32(this, 0x24) = 0;
    u8(this, 0x34) = 0;
}

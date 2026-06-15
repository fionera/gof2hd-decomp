#include "gof2/engine/render/LodMeshMerger.h"
#include "gof2/engine/math/AEMath.h"
#include "gof2/game/core/PaintCanvasClass.h"
#include "gof2/platform/libc.h"

namespace AbyssEngine {

void LodMeshMerger::setEnabled(int index, bool value)
{
    if (enabled[index] != value) {
        enabled[index] = value;
        if (visible[index] != 0) {
            dirty = 1;
        }
    }
}

void LodMeshMerger::setLod(int index, signed char lod)
{
    if (lodLevels[index] != lod) {
        lodLevels[index] = lod;
        if (enabled[index] != 0) {
            dirty = 1;
        }
    }
}

// Store the source transform into its per-row matrix slot.
void LodMeshMerger::setMatrix(int index, const Matrix& m)
{
    transforms[index] = m;
}

void LodMeshMerger::setMesh(int index, signed char lod, uint16_t meshId)
{
    uint32_t id;
    canvas->MeshCreate(meshId, &id, false);
    void* ptr = canvas->MeshGetPointer(id);
    sourceMeshes.data()[rows * lod + index] = (Mesh*)ptr;
}

void LodMeshMerger::update()
{
    for (int i = 0; i < rows; i++) {
        void* sph = transformedMeshes[i];
        uint8_t vis = (uint8_t)canvas->CameraIsSphereinViewFrustum(
            (char*)sph + 0x3c,
            *(float*)((char*)sph + 0x48));
        if (vis != visible[i]) {
            visible[i] = vis;
            if (enabled[i] != 0) {
                dirty = 1;
            }
        }
    }

    if (dirty == 0) {
        return;
    }

    // Sum the index counts of every active LOD slice.
    int indexBudget = 0;
    for (int j = 0; j < rows; j++) {
        if (enabled[j] != 0 && visible[j] != 0) {
            signed char lod = lodLevels[j];
            Mesh* src = (Mesh*)transformedMeshes[rows * lod + j];
            indexBudget += src->indexCount;
        }
    }

    // While over budget, drop to coarser LODs.
    for (int j = 0; indexBudget >= 0x10000 && j < rows; j++) {
        if (enabled[j] != 0 && visible[j] != 0) {
            signed char lod = lodLevels[j];
            if (lod < cols - 1) {
                Mesh* prev = (Mesh*)transformedMeshes[rows * lod + j];
                setLod(j, (signed char)(lod + 1));
                signed char newLod = lodLevels[j];
                Mesh* cur = (Mesh*)transformedMeshes[rows * newLod + j];
                indexBudget += cur->indexCount - prev->indexCount;
            }
        }
    }

    // Copy each active slice into the merged mesh.
    uint8_t* out = (uint8_t*)mergedMesh;
    int vtxOffset = 0;
    int idxOffset = 0;
    for (int j = 0; j < rows; j++) {
        if (enabled[j] != 0 && visible[j] != 0) {
            uint8_t mask = out[0];
            signed char lod = lodLevels[j];
            Mesh* src = (Mesh*)transformedMeshes[rows * lod + j];

            if (mask & 1) {
                memcpy(*(char**)(out + 4) + vtxOffset * 0xc,
                       src->positions, src->vertexCount * 0xc);
            }
            if (mask & 4) {
                memcpy(*(char**)(out + 0x10) + vtxOffset * 0xc,
                       src->normals, src->vertexCount * 0xc);
            }
            if (mask & 8) {
                memcpy(*(char**)(out + 0xc) + vtxOffset * 0x10,
                       (void*)(uintptr_t)src->colors, (uint32_t)src->vertexCount << 4);
            }
            if (mask & 2) {
                memcpy(*(char**)(out + 8) + vtxOffset * 8,
                       src->texCoords, (uint32_t)src->vertexCount << 3);
            }
            if (mask & 0x10) {
                const int16_t* si = (const int16_t*)src->indices;
                int16_t* di = (int16_t*)(*(char**)(out + 0x2c) + idxOffset * 2);
                for (uint16_t k = 0; k < src->indexCount; k++) {
                    di[k] = (int16_t)(si[k] + (int16_t)vtxOffset);
                }
            }
            idxOffset += src->indexCount;
            vtxOffset += src->vertexCount;
        }
    }
    *(int16_t*)(out + 0x28) = (int16_t)idxOffset;
    *(int16_t*)(out + 2) = (int16_t)vtxOffset;
    dirty = 0;
}

LodMeshMerger::LodMeshMerger(int rows_, int cols_, PaintCanvas* canvas_, uint16_t flags_)
    : rows(rows_),
      flags(flags_),
      initialized(0),
      canvas(canvas_),
      mergedMesh(nullptr),
      transformedMeshes(nullptr),
      transforms(nullptr),
      lodLevels(nullptr),
      cols(cols_),
      dirty(0)
{
    sourceMeshes.resize((uint32_t)(cols * rows));

    uint32_t n = (uint32_t)rows;
    transformedMeshes = new void*[n * cols]();

    lodLevels = new int8_t[n]();

    transforms = new Matrix[n];
    for (uint32_t i = 0; i < n; i++) {
        // 15-float packed affine identity.
        Matrix tmp;
        tmp.m[0] = 1.0f;
        tmp.m[1] = 0.0f; tmp.m[2] = 0.0f; tmp.m[3] = 0.0f;
        tmp.m[4] = 1.0f;
        tmp.m[5] = 0.0f; tmp.m[6] = 0.0f; tmp.m[7] = 0.0f; tmp.m[8] = 0.0f;
        tmp.m[9] = 0.0f;
        tmp.m[10] = 1.0f;
        tmp.m[11] = 0.0f; tmp.m[12] = 0.0f; tmp.m[13] = 0.0f;
        tmp.m[14] = 1.0f;
        transforms[i] = tmp;
    }

    enabled = new uint8_t[n];
    for (uint32_t i = 0; i < n; i++) enabled[i] = 1;

    visible = new uint8_t[n];
    for (uint32_t i = 0; i < n; i++) visible[i] = 1;
}

int LodMeshMerger::init()
{
    if (initialized != 0) {
        return initialized;
    }

    for (int i = 0; i < rows; i++) {
        int lod = lodLevels[i];
        if (lod >= -1 && cols <= lod) {
            lodLevels[i] = 0;
        }
        for (int c = 0; c < cols; c++) {
            Mesh* mesh = sourceMeshes.data()[rows * c + i];
            if (mesh != nullptr) {
                transformedMeshes[rows * c + i] = transformMesh(mesh, transforms[i]);
            }
        }
    }

    uint16_t nv = 0;
    uint16_t ni = 0;
    for (int i = 0; i < rows; i++) {
        Mesh* m0 = sourceMeshes.data()[i];
        nv = nv + m0->vertexCount;
        ni = ni + (uint16_t)(m0->indexCount / 3);
    }

    canvas->MeshCreate(ni, nv,
                       (signed char)sourceMeshes.data()[0]->vertexFormat,
                       flags, &mergedMeshId);
    mergedMesh = canvas->MeshGetPointer(mergedMeshId);
    canvas->TransformCreate(&transformId);
    canvas->TransformAddMeshId(transformId, mergedMeshId);
    dirty = 1;

    // Flag the merger as built and run the first merge pass (dirty was just set,
    // so update() copies every enabled LOD slice into the merged mesh).
    initialized = 1;
    update();
    return initialized;
}

// Builds a fresh Mesh that is `src` with all geometry transformed by `m`: positions
// are transformed, normals rotated+normalised, and the bounding sphere recomputed.
void* LodMeshMerger::transformMesh(Mesh* src, const Matrix& m)
{
    char* s = (char*)src;
    char* out = (char*)::operator new(0x88);

    // zero-initialise the new mesh, then set the default bsphere radius.
    memset(out, 0, 0x88);
    *(float*)(out + 0x4c) = 1.0f;

    // copy counts/flags from source.
    uint32_t vcount = *(uint16_t*)(s + 0x2);
    *(uint16_t*)(out + 0x2) = *(uint16_t*)(s + 0x2);
    *(uint16_t*)(out + 0x28) = *(uint16_t*)(s + 0x28);
    uint8_t f = *(uint8_t*)s;
    *(uint8_t*)out = f;

    // conditional pointer copies based on flag bits (texcoords / colours / indices).
    if (f & 0x2)
        *(uint32_t*)(out + 0x8) = *(uint32_t*)(s + 0x8);
    if (f & 0x10)
        *(uint32_t*)(out + 0xc) = *(uint32_t*)(s + 0xc);
    if (f & 0x20)
        *(uint32_t*)(out + 0x2c) = *(uint32_t*)(s + 0x2c);

    // transform vertex positions.
    if (f & 0x1) {
        char* dstV = new char[vcount * 0xc];
        *(void**)(out + 0x4) = dstV;
        int o = 0;
        for (uint32_t i = 0; i < vcount; i++) {
            *(Vector*)(*(char**)(out + 0x4) + o) =
                AEMath::MatrixTransformVector(m, *(Vector*)(*(char**)(s + 0x4) + o));
            o += 0xc;
            vcount = *(uint16_t*)(s + 0x2);
        }
        f = *(uint8_t*)s;
    }

    // rotate + normalise normals.
    if (f & 0x4) {
        char* dstN = new char[vcount * 0xc];
        *(void**)(out + 0x10) = dstN;
        int o = 0;
        for (uint32_t i = 0; i < vcount; i++) {
            Vector rot = AEMath::MatrixRotateVector(m, *(Vector*)(*(char**)(s + 0x10) + o));
            *(Vector*)(*(char**)(out + 0x10) + o) = AEMath::VectorNormalize(rot);
            o += 0xc;
            vcount = *(uint16_t*)(s + 0x2);
        }
    }

    // recompute bounding sphere: transform the centre, then derive a conservative
    // radius from the transformed extents.
    float r = *(float*)(s + 0x48);
    Vector ext = { r, r, r };
    Vector tExt = AEMath::MatrixTransformVector(m, ext);
    Vector center = AEMath::MatrixTransformVector(m, *(Vector*)(s + 0x3c));

    float ax = tExt.x < 0.0f ? -tExt.x : tExt.x;
    float ay = tExt.y < 0.0f ? -tExt.y : tExt.y;
    float az = tExt.z < 0.0f ? -tExt.z : tExt.z;
    float rad = ax;
    if (ay > rad) rad = ay;
    if (az > rad) rad = az;

    *(float*)(out + 0x3c) = center.x;
    *(float*)(out + 0x40) = center.y;
    *(float*)(out + 0x44) = center.z;
    *(float*)(out + 0x48) = rad;

    return out;
}

// Per-array-slot release thunk read from a hidden PC-relative global (used to free
// the lod/enabled/visible byte arrays).
__attribute__((visibility("hidden"))) extern void (*const g_freeFn)(void*);

LodMeshMerger::~LodMeshMerger()
{
    // Release every transformed mesh (its position/normal buffers, then the mesh).
    int count = rows * cols;
    for (int i = 0; i < count; i++) {
        char* cell = (char*)transformedMeshes[i];
        if (cell != nullptr) {
            void** positions = (void**)(cell + 4);
            if (*positions != nullptr) {
                ::operator delete[](*positions);
                *positions = nullptr;
            }
            void** normals = (void**)(cell + 0x10);
            if (*normals != nullptr) {
                ::operator delete[](*normals);
                *normals = nullptr;
            }
            ::operator delete(cell);
            transformedMeshes[i] = nullptr;
        }
    }
    delete[] transformedMeshes;
    transformedMeshes = nullptr;

    g_freeFn(lodLevels);
    lodLevels = nullptr;
    g_freeFn(enabled);
    enabled = nullptr;
    g_freeFn(visible);
    visible = nullptr;

    delete[] transforms;
    transforms = nullptr;

    // Release the embedded source-mesh array's backing buffer.
    sourceMeshes.clear();
    sourceMeshes.shrink_to_fit();
}

} // namespace AbyssEngine

#include "engine/render/SimpleMeshMerger.h"
#include "engine/render/Mesh.h"

namespace AbyssEngine {

using AEMath::Matrix;
using AEMath::Vector;

// Engine helpers backing the merge: source-mesh loading, merged-mesh allocation,
// per-vertex/-triangle writes, and transform creation. These resolve to PaintCanvas /
// Mesh / Matrix primitives in libgof2hd; declared here until the engine externs pass
// gives them their real homes.
extern "C" void  SMM_MeshCreate_fromId(void* canvas, unsigned short id, unsigned* out, bool b);
extern "C" void* SMM_MeshGetPointer(void* canvas, unsigned meshId);
extern "C" unsigned SMM_uidiv(unsigned a, unsigned b);
extern "C" void  SMM_MatrixTransformVector(Matrix* out, const Vector* v);
extern "C" void  SMM_MatrixRotateVector(Matrix* out, const Vector* v);
extern "C" void  SMM_MeshSetNormal(void* canvas, unsigned meshId, unsigned short idx, Vector* n);
extern "C" void  SMM_TransformCreate(void* canvas, unsigned* out);
extern "C" void  SMM_TransformAddMeshId(void* canvas, unsigned transformId, unsigned meshId);
extern "C" void  SMM_MeshCreate_empty(void* canvas, unsigned short verts, unsigned short tris,
                                      int firstFlags, float factor, unsigned* out);
extern "C" void  SMM_MeshSetPoint(unsigned canvas, unsigned short meshId, unsigned short idx,
                                  float x, float y, float z);
extern "C" void  SMM_MeshSetUv(unsigned canvas, unsigned short meshId, unsigned short idx,
                               float u, float v);
extern "C" void  SMM_MeshSetColor(void* canvas, unsigned meshId, unsigned short idx,
                                  float r, float g, float b, float a);
extern "C" void  SMM_MeshSetTriangle(unsigned canvas, unsigned short meshId, unsigned short tri,
                                     unsigned short a, unsigned short b, unsigned short c);

// Builds a single merged mesh from `meshIds`, each instance placed by the matching
// matrix row in `transforms`. Loads every source mesh, allocates one merged mesh sized
// to the combined vertex/triangle counts, copies every source vertex (position/normal/
// uv/color) transformed by its per-mesh matrix and every source triangle (index-rebased),
// then builds a transform holding the merged mesh.
SimpleMeshMerger::SimpleMeshMerger(unsigned* meshIds, unsigned* transforms,
                                   PaintCanvas* canvas, float factor) {
    void* cv = canvas;
    this->mergeFactor = (short)factor;
    this->canvas = canvas;
    this->matrixCount = (int)transforms[0];
    this->meshes.resize(meshIds[0]);

    // pass 1: load source meshes, accumulate vertex + triangle totals
    short vertTotal = 0;
    short triTotal = 0;
    for (unsigned i = 0; i < meshIds[0]; ++i) {
        unsigned newId;
        SMM_MeshCreate_fromId(cv, *(unsigned short*)(meshIds[1] + i * 2), &newId, false);
        this->meshes[i] = (Mesh*)SMM_MeshGetPointer(cv, newId);
        Mesh* m = this->meshes[i];
        short vc = (short)m->vertexCount;
        short tc = (short)SMM_uidiv(m->indexCount, 3);
        triTotal = triTotal + vc;       // (preserves the original accumulation order)
        vertTotal = vertTotal + tc;
    }

    // allocate the merged mesh, seeding its vertex-format flags from the first source mesh
    Mesh* firstMesh = this->meshes[0];
    unsigned mergedId;
    SMM_MeshCreate_empty(cv, (unsigned short)vertTotal, (unsigned short)triTotal,
                         (int)(signed char)firstMesh->vertexFormat, factor, &mergedId);
    this->mergedMeshId = mergedId;

    // pass 2: copy transformed vertices and rebased triangles
    short vBase = 0;
    short tBase = 0;
    for (unsigned i = 0; i < meshIds[0]; ++i) {
        Mesh* m = this->meshes[i];
        int uvOff = 0, colOff = 0;
        unsigned char flags = 0;
        for (unsigned v = 0; v < m->vertexCount; ++v) {
            flags = m->vertexFormat;
            if (flags & 1) {   // has positions
                Vector xform;
                const Vector* src = (const Vector*)(transforms[1] + i * 0x3c);
                SMM_MatrixTransformVector((Matrix*)&xform, src);
                SMM_MeshSetPoint((unsigned)(unsigned long)cv,
                                 (unsigned short)this->mergedMeshId,
                                 (unsigned short)(vBase + (short)v),
                                 xform.x, xform.y, xform.z);
                flags = m->vertexFormat;
            }
            if (flags & 4) {   // has normals
                Matrix rotated;
                const Vector* src = (const Vector*)(transforms[1] + i * 0x3c);
                SMM_MatrixRotateVector(&rotated, src);
                SMM_MeshSetNormal(cv, this->mergedMeshId,
                                  (unsigned short)(vBase + (short)v), (Vector*)&rotated);
                flags = m->vertexFormat;
            }
            if (flags & 2) {   // has uvs
                float* uv = (float*)((char*)m->texCoords + uvOff);
                SMM_MeshSetUv(0 /*canvas-as-uint*/, (unsigned short)this->mergedMeshId,
                              (unsigned short)(vBase + (short)v), uv[1], 0.0f);
                flags = m->vertexFormat;
            }
            if (flags & 8) {   // has colors
                float* col = (float*)((char*)(uintptr_t)m->colors + colOff);
                SMM_MeshSetColor(cv, this->mergedMeshId,
                                 (unsigned short)(vBase + (short)v),
                                 col[1], col[2], col[3], 0.0f);
            }
            uvOff = uvOff + 8;
            colOff = colOff + 0x10;
        }

        unsigned triCount = SMM_uidiv(m->indexCount, 3);
        int triOff = 0;
        for (unsigned t = 0; t < triCount; ++t) {
            if (m->vertexFormat & 0x10) {   // has triangles
                short* tri = (short*)((char*)m->indices + triOff);
                unsigned short a = (unsigned short)(tri[0] + vBase);
                unsigned short b = (unsigned short)(tri[1] + vBase);
                unsigned short c = (unsigned short)(tri[2] + vBase);
                SMM_MeshSetTriangle(0 /*canvas-as-uint*/, (unsigned short)this->mergedMeshId,
                                    (unsigned short)(tBase + (short)t), a, b, c);
            }
            triOff = triOff + 6;
        }

        tBase = tBase + (short)SMM_uidiv(m->indexCount, 3);
        vBase = vBase + (short)m->vertexCount;
    }

    SMM_TransformCreate(cv, &this->transformId);
    SMM_TransformAddMeshId(cv, this->transformId, this->mergedMeshId);
    this->valid = 1;
}

} // namespace AbyssEngine

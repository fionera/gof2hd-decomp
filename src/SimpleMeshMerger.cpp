#include "gof2/SimpleMeshMerger.h"


extern "C" void SMM_Array_Mesh_ctor(void *arr);
extern "C" void SMM_ArraySetLength_Mesh(unsigned n, void *arr);
extern "C" void SMM_MeshCreate_fromId(void *canvas, unsigned short id, unsigned *out, bool b);
extern "C" void *SMM_MeshGetPointer(void *canvas, unsigned meshId);
extern "C" unsigned SMM_uidiv(unsigned a, unsigned b);
extern "C" void SMM_MatrixTransformVector(Matrix *out, const Vector *v);
extern "C" void SMM_MatrixRotateVector(Matrix *out, const Vector *v);
extern "C" void SMM_MeshSetNormal(void *canvas, unsigned meshId, unsigned short idx, Vector *n);
extern "C" void SMM_TransformCreate(void *canvas, unsigned *out);
extern "C" void SMM_TransformAddMeshId(void *canvas, unsigned transformId, unsigned meshId);
extern "C" void SMM___stack_chk_fail();

// ---- SimpleMeshMerger_180ecc.cpp ----
// SimpleMeshMerger::SimpleMeshMerger(const Array<unsigned short>& meshIds,
//                                    Array<Matrix> transforms, PaintCanvas* canvas, ushort factor)
// Loads each source mesh, allocates one merged mesh sized to the combined vertex/triangle
// counts, then copies every source vertex (position/normal/uv/color) transformed by its
// per-mesh matrix and every source triangle (index-rebased) into the merged mesh. Finally
// builds a transform holding the merged mesh.
//
// `transforms` arrives by value; the decompile reads it as `undefined4* param_3` whose [1]
// member is the matrix data pointer (rows of 0x3c bytes). We model it the same way.

extern "C" void SMM_MeshCreate_empty(void *canvas, unsigned short verts, unsigned short tris,
                                     int firstFlags, float factor, unsigned *out);
extern "C" void SMM_MeshSetPoint(unsigned canvas, unsigned short meshId, unsigned short idx,
                                 float x, float y, float z);
extern "C" void SMM_MeshSetUv(unsigned canvas, unsigned short meshId, unsigned short idx,
                              float u, float v);
extern "C" void SMM_MeshSetColor(void *canvas, unsigned meshId, unsigned short idx,
                                 float r, float g, float b, float a);
extern "C" void SMM_MeshSetTriangle(unsigned canvas, unsigned short meshId, unsigned short tri,
                                    unsigned short a, unsigned short b, unsigned short c);

__attribute__((visibility("hidden"))) extern int **g_SMM_canary;

extern "C" SimpleMeshMerger *SimpleMeshMerger_ctor(SimpleMeshMerger *self, unsigned *meshIds,
                                                   unsigned *transforms, void *canvas,
                                                   float factor)
{
    int *canary = *g_SMM_canary;
    int saved = *canary;

    SMM_Array_Mesh_ctor((char *)self + 8);
    self->f_4 = (short)factor;
    self->f_14 = canvas;
    self->f_0 = *(int *)transforms;
    SMM_ArraySetLength_Mesh(meshIds[0], (char *)self + 8);

    // pass 1: load source meshes, accumulate vertex + triangle totals
    short vertTotal = 0;
    short triTotal = 0;
    for (unsigned i = 0; i < meshIds[0]; i = i + 1) {
        unsigned newId;
        SMM_MeshCreate_fromId(canvas, *(unsigned short *)(meshIds[1] + i * 2), &newId, false);
        void *meshPtr = SMM_MeshGetPointer(canvas, newId);
        ((void **)self->f_c)[i] = meshPtr;
        char *m = (char *)((void **)self->f_c)[i];
        short vc = *(short *)(m + 2);
        short tc = (short)SMM_uidiv(*(unsigned short *)(m + 0x28), 3);
        triTotal = triTotal + vc;       // (matches target's accumulation order)
        vertTotal = vertTotal + tc;
    }

    // allocate the merged mesh
    char *firstMesh = (char *)self->f_c;
    unsigned mergedId;
    SMM_MeshCreate_empty(canvas, (unsigned short)vertTotal, (unsigned short)triTotal,
                         (int)*(signed char *)firstMesh, factor, &mergedId);
    self->f_18 = mergedId;

    // pass 2: copy transformed vertices and rebased triangles
    short vBase = 0;
    short tBase = 0;
    for (unsigned i = 0; i < meshIds[0]; i = i + 1) {
        char *m = (char *)((void **)self->f_c)[i];
        int posOff = 0, uvOff = 0, colOff = 0;
        unsigned v = 0;
        unsigned char flags = 0;
        for (; v < *(unsigned short *)(m + 2); v = v + 1) {
            m = (char *)((void **)self->f_c)[i];
            flags = *(unsigned char *)m;
            // transformed-vector scratch (matrix-transform writes a Vector into the head)
            float xform[4];
            Matrix *matOut = (Matrix *)xform;
            if (flags & 1) {
                Vector *src = (Vector *)(transforms[1] + i * 0x3c);
                SMM_MatrixTransformVector(matOut, src);
                SMM_MeshSetPoint((unsigned)(unsigned long)canvas,
                                 (unsigned short)self->f_18,
                                 (unsigned short)(vBase + (short)v),
                                 xform[0], xform[1], xform[2]);
                m = (char *)((void **)self->f_c)[i];
                flags = *(unsigned char *)m;
            }
            Matrix mat;
            if ((unsigned)flags & 4) {   // (flags<<0x1d)<0 -> bit2
                Vector *src = (Vector *)(transforms[1] + i * 0x3c);
                SMM_MatrixRotateVector(&mat, src);
                SMM_MeshSetNormal(canvas, self->f_18,
                                  (unsigned short)(vBase + (short)v), (Vector *)&mat);
                m = (char *)((void **)self->f_c)[i];
                flags = *(unsigned char *)m;
            }
            if ((unsigned)flags & 2) {   // (flags<<0x1e)<0 -> bit1
                float *uv = (float *)(*(int *)(m + 8) + uvOff);
                SMM_MeshSetUv(0 /*canvas-as-uint*/, (unsigned short)self->f_18,
                              (unsigned short)(vBase + (short)v), uv[1], 0.0f);
                m = (char *)((void **)self->f_c)[i];
                flags = *(unsigned char *)m;
            }
            if ((unsigned)flags & 8) {   // (flags<<0x1c)<0 -> bit3
                float *col = (float *)(*(int *)(m + 0xc) + colOff);
                SMM_MeshSetColor(canvas, self->f_18,
                                 (unsigned short)(vBase + (short)v),
                                 col[1], col[2], col[3], 0.0f);
            }
            posOff = posOff + 0xc;
            uvOff = uvOff + 8;
            colOff = colOff + 0x10;
        }

        unsigned triCount = SMM_uidiv(*(unsigned short *)(m + 0x28), 3);
        int triOff = 0;
        for (unsigned t = 0; t < triCount; t = t + 1) {
            if ((unsigned)*(unsigned char *)m & 0x10) {   // (flags<<0x1b)<0 -> bit4
                short *tri = (short *)(*(int *)(m + 0x2c) + triOff);
                unsigned short a = (unsigned short)(tri[0] + vBase);
                unsigned short b = (unsigned short)(tri[1] + vBase);
                unsigned short c = (unsigned short)(tri[2] + vBase);
                SMM_MeshSetTriangle(0 /*canvas-as-uint*/, (unsigned short)self->f_18,
                                    (unsigned short)(tBase + (short)t), a, b, c);
                m = (char *)((void **)self->f_c)[i];
            }
            triOff = triOff + 6;
        }

        tBase = tBase + (short)SMM_uidiv(*(unsigned short *)(m + 0x28), 3);
        vBase = vBase + *(short *)(m + 2);
    }

    SMM_TransformCreate(canvas, (unsigned *)((char *)self + 0x1c));
    SMM_TransformAddMeshId(canvas, self->f_1c, self->f_18);
    self->field_0x6 = 1;

    if (*canary != saved) {
        SMM___stack_chk_fail();
    }
    return self;
}
